#ifndef __MY_SQLITE__
#define __MY_SQLITE__

#include <sqlite3.h>
#include "my_array.h"
#include "my_assoc.h"

// ***************************************************
//  typedef struct sqlite
/*!
    @brief Resultset of SQLite.
*/
// ***************************************************
typedef struct
{
  //! Database what we use
  sqlite3 *db;

  //! Array where we store all found results
  a_array values;

  //! Array where we store field names
  a_array fields;

  //! Query what we called last time
  char *last_query;

  //! Debug information for our callback function. This prevents
  //! adding field names every time when we call callback function.
  int field_names_added;

  //! How many results we found
  int num_items;

  //! How many fields we have
  int num_fields;

  //! Have we connected or not
  int connected;

  //! Last error
  char *error;

  //! Do we have error message in buffer?
  int error_set;

} a_sqlite;

void sqlite_connect( a_sqlite *tmp, char *database );
int sqlite_callback( void *pArg, int argc, char **argv, 
  char **column_names );
void sqlite_query( a_sqlite *database, char *query );
void sqlite_free( a_sqlite *database );
a_sqlite sqlite_init();
void sqlite_free_resultset( a_sqlite *database );
a_assoc sqlite_fetch_assoc( a_sqlite *db );
int sqlite_num_rows( a_sqlite *db );
int sqlite_num_fields( a_sqlite *db );
char *sqlite_fetch_field( a_sqlite *db, int field_num );
char **sqlite_fetch_row( a_sqlite *db, int row_number );

#endif
