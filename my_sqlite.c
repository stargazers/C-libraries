#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include "my_sqlite.h"
#include "my_debug.h"

// ***************************************************
//  sqlite_connect
/*!
    @brief Connect to sqlite file.

    @param a_sqlite *tmp Pointer to a_sqlite typed struct.
           Here we save connection info.

    @param char *database Database to connect.
*/
// ***************************************************
void sqlite_connect( a_sqlite *tmp, char *database )
{
  debug_print( DP_MESSAGE, "IN FUNCTION: sqlite_connect" );
  int rc;

  // Try to connect
  rc = sqlite3_open( database, &tmp->db );

  // If we failed to connect.
  if( rc )
  {
    debug_print( DP_DEBUG, "Failed to connect!\n" );
    tmp->connected = 0;
  }
  else
  {
    // Now we must tell that we are connected
    tmp->connected = 1;
  }
}

// ***************************************************
//  sqlite_callback
/*!
    @brief Callback function what is called every time
           when user calls sqlite_query(). This should
           not be called by user directly!

    @param void *pArg. Pointer to a_sqlite struct
           where we store return values.

    @param int argc Number of return values of query.

    @param char **argv Values what sqlite have returned.

    @param char **column_names Column names what we
           got on last query.

    @return int 0.
*/
// ***************************************************
int sqlite_callback( void *pArg, int argc, char **argv, 
  char **column_names )
{
  debug_print( DP_MESSAGE, "IN FUNCTION: sqlite_callback" );
  int i;

  // Convert void *pArg to a_sqlite type. There we store
  // information about this query results.
  a_sqlite *tmp = (a_sqlite *) pArg;

  // Loop as many times as the number of found items in last query.
  for( i=0; i < argc; i++ )
    array_add( &tmp->values, argv[i] );

  // If we haven't set field names to array fields, then
  // add them now and set flag field_names_added to 1 so we don't
  // call it next time in same query results.
  if(! tmp->field_names_added )
  {
    for( i=0; i < argc; i++ )
      array_add( &tmp->fields, column_names[i] );

    tmp->field_names_added = 1;
  }

  return 0;
}

// ***************************************************
//  sqlite_query
/*!
    @brief Execute a SQL query.

    @param a_sqlite *database Struct where we store
           our query results.

    @param char *query Query to run.
*/
// ***************************************************
void sqlite_query( a_sqlite *database, char *query )
{
  debug_print( DP_MESSAGE, "IN FUNCTION: sqlite_query" );
  debug_print( DP_DEBUG, query );

  // If we have no open connection, exit this function.
  if(! database->connected )
  {
    debug_print( DP_DEBUG, "Not connected to database!" );
    return;
  }

  int rc;
  sqlite_free_resultset( database );

  database->field_names_added = 0;

  a_array results_array;
  a_array fields_array;

  results_array = array_init();
  fields_array = array_init();

  array_free( &database->fields );
  array_free( &database->values );

  database->fields = fields_array;
  database->values = results_array;

  // Execute query and call our sqlite_callback function.
  // Also, pass a_sqlite *database as an parameter to our
  // callback function, so we can store return values there.
  rc = sqlite3_exec( database->db, query, sqlite_callback,
    &(*database), &database->error );

  // If query failed, put error_set flag to 1 and num_items to 0
  if( rc != SQLITE_OK )
  {
    database->error_set = 1;
    database->num_items = 0;
  }
  else
  {
    debug_print( DP_MESSAGE, "Query executed fine." );
  }

  // Allocate memory for last query
  char *tmp = malloc( strlen( query ) +1 );
  
  if( tmp == NULL )
  {
    printf( "Malloc fails!\n" );
    exit(1);
  }

  strcpy( tmp, query );

  // Update info what we called last time
  database->last_query = tmp;

  int num_results = database->values.num_items;
  int num_fields = database->fields.num_items;

  database->num_fields = database->fields.num_items;

  // Update info about how many rows we have. 
  // Remember to check if there is more than 0 fields. If there is
  // no more fields than 0, then just don't divide by zero. 
  // Number of items should be 0.
  if( num_fields > 0 )
    database->num_items = num_results / num_fields;
  else
    database->num_items = 0;
}

// ***************************************************
//  sqlite_free_resultset
/*!
    @brief Free sqlite3 resultset. Should be called
           after every query before new query. 
           User should not call this function!
           Use sqlite_free() instead.

    @param a_sqlite *database Our database struct
*/
// ***************************************************
void sqlite_free_resultset( a_sqlite *database )
{
  debug_print( DP_MESSAGE, "IN FUNCTION: sqlite_free_resultset" );

  // Free memory of last query string
  free( database->last_query );

  // If there was error, we must release allocated memory
  // of error message using sqlite3_free, not normal free!
  if( database->error_set )
  {
    printf( "%s\n", database->error );
    sqlite3_free( database->error );
  }
}

// ***************************************************
//  sqlite_free
/*!
    @brief Free sqlite3 results

    @param a_sqlite *database Our database struct
*/
// ***************************************************
void sqlite_free( a_sqlite *database )
{
  debug_print( DP_MESSAGE, "IN FUNCTION: sqlite_free" );

  sqlite_free_resultset( database );

  // Free array of all results
  array_free( &database->values );
  array_free( &database->fields );

  // Free memory only if we are connected!
  if( database->connected )
    sqlite3_close( database->db );
}

// ***************************************************
//  sqlite_init
/*!
    @brief Creates an a_sqlite struct and returns it.

    @return a_sqlite Our database struct.
*/
// ***************************************************
a_sqlite sqlite_init()
{
  debug_print( DP_MESSAGE, "IN FUNCTION: sqlite_init" );
  a_sqlite tmp;

  tmp.num_items = 1;
  tmp.connected = 0;
  tmp.field_names_added = 0;

  tmp.last_query = malloc( 1 );

  if( tmp.last_query == NULL )
    malloc_failed();

  tmp.error_set = 0;

  tmp.values = array_init();
  tmp.fields = array_init();

  return tmp;
}

// ***************************************************
//  sqlite_fetch_assoc
/*!
    @brief Fetch sqlite result row to assoc array

    @param a_sqlite *db SQLite struct.

    @return a_assoc Array where key is field name.
*/
// ***************************************************
a_assoc sqlite_fetch_assoc( a_sqlite *db )
{
  a_assoc tmp = assoc_init();
  int i;

  // Loop as many times as there was fields and
  // add to assoc array as key = fieldname, value = value
  for( i=0; i < db->fields.num_items; i++ )
    assoc_add( &tmp, db->fields.values[i], db->values.values[i] );

  return tmp;
}

// ***************************************************
//  sqlite_fetch_row
/*!
    @brief Fetch sqlite result row to basic char array

    @param a_sqlite *db SQLite struct.

    @param int row_number Resultset row number to fetch

    @return char** Array of strings. This MUST be freed!
*/
// ***************************************************
char **sqlite_fetch_row( a_sqlite *db, int row_number )
{
  debug_print( DP_MESSAGE, "In function: sqlite_fetch_row" );

  int new_size = sizeof( char * ) * db->num_fields + 1;
  int i;
  char **tmp = malloc( new_size );

  if( tmp == NULL )
    malloc_failed();
  
  // Start point of the loop is row number * number of fields
  int start = db->num_fields * row_number;
  int end = start + db->num_fields;
  int counter = 0;

  // Add values
  for( i=start; i < end; i++ )
  {
    tmp[counter] = db->values.values[i];
    counter++;
  }

  return tmp;
}


// ***************************************************
//  sqlite_num_rows
/*!
    @brief Return number of rows of last query

    @param a_sqlite *db SQLite struct.

    @return int Number of rows. This is just a other 
            way to db->num_items.
*/
// ***************************************************
int sqlite_num_rows( a_sqlite *db )
{
  return db->num_items;
}

// ***************************************************
//  sqlite_num_fields
/*!
    @brief Return number of fields of last query

    @param a_sqlite *db SQLite struct.

    @return int Number of fields. This is just a other 
            way to db->num_fields.
*/
// ***************************************************
int sqlite_num_fields( a_sqlite *db )
{ 
  return db->num_fields;
}

// ***************************************************
//  sqlite_fetch_field
/*!
    @brief Returns name of a field in index field_num

    @param a_sqlite *db SQLite struct.

    @param int field_num Nuber of wanted field

    @return char * Pointer to a field name. 
            This must NOT to be freed.
*/
// ***************************************************
char *sqlite_fetch_field( a_sqlite *db, int field_num )
{
  return db->fields.values[ field_num ];
}
