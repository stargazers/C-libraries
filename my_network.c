#include <stdio.h>
#include <stdlib.h>
#include "my_array.h"
#include "my_string.h"
#include "my_network.h"

// ***************************************************
//  get_post_values
/*!
    @brief Get POST-values to arrays. This automatically
           fix values from hexacodes to normal characters.

    @param a_array *keys Array where we store keys

    @param a_array *values Array where we store values
*/
// ***************************************************
void get_post_values( a_array *keys, a_array *values )
{
  char *lenstr;
  char input[1024];
  long len;
  int i;

  // Read the length of POST-datas
  lenstr = getenv( "CONTENT_LENGTH" );

  // Read length of the content and store it to
  // variable "len" as an long value.
  sscanf( lenstr, "%ld", &len );

  // Read data from the standard input to variable "input".
  fgets( input, len + 1, stdin );

  // Create arrays for keys and values
  create_arrays( input, keys, values );

  // Temporary arrays for fixed values
  a_array tmp_values = array_init();
  a_array tmp_keys = array_init();

  for( i=0; i < keys->num_items; i++ )
  {
    char tmp[100];
    fix_chars( keys->values[i], tmp );
    array_add( &tmp_keys, tmp );

    fix_chars( values->values[i], tmp );
    array_add( &tmp_values, tmp );
  }

  // Remove arrays with uncorrected values
  array_free( values );
  array_free( keys );

  // Point keys and values to arrays with correct values
  *values = tmp_values;
  *keys = tmp_keys;
}

// ***************************************************
//  create_arrays
/*!
    @brief Create array of POST-datas, eg. creates
           two arrays. One for keys and another
           for values.
    
    @param char *data POST-data as string

    @param a_array *keys Array where we store found keys

    @param a_array *values Array where we store values
*/
// ***************************************************
void create_arrays( char *data, a_array *keys, a_array *values )
{
  a_array tmp = array_init();
  int i;

  // Split items using & as delimiter.
  explode( "&", data, &tmp );

  // Loop all items
  for( i=0; i < tmp.num_items; i++ )
  {
    // Temporary array
    a_array inner = array_init();
    explode( "=", tmp.values[i], &inner );

    // Left side of = char is KEY.
    array_add( keys, inner.values[0] );

    // If value is NULL, then we add "" in the array.
    // Otherwise we add right side of the = character.
    if( inner.values[1] != NULL )
      array_add( values, inner.values[1] );
    else
      array_add( values, "" );

    array_free( &inner );
  }
}
