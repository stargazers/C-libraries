#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_array.h"
#include "my_assoc.h"

// ***************************************************
//  assoc_init
/*!
    @brief Initialize assoc arary

    @param a_assoc *assoc Assoc array
*/
// ***************************************************
a_assoc assoc_init()
{
  a_assoc tmp;

  a_array keys;
  keys = array_init();

  a_array values;
  values = array_init();

  tmp.keys = keys;
  tmp.values = values;

  tmp.num_items = 0;

  return tmp;
}

// ***************************************************
//  assoc_free
/*!
    @brief Frees memory of assoc array

    @param a_assoc *assoc Array of values
*/
// ***************************************************
void assoc_free( a_assoc *assoc )
{
  array_free( &assoc->keys );
  array_free( &assoc->values );
}

// ***************************************************
//  assoc_add
/*!
    @brief Add value to an assoc array

    @param a_assoc *assoc Array of values

    @param char *key Key to value

    @param char *value Value 
*/
// ***************************************************
void assoc_add( a_assoc *assoc, char *key, char *value )
{
  // First search if key exists.
  int pos = array_pos( &assoc->keys, key );

  // Key did not exists. Create it.
  if( pos == -1 )
  {
    array_add( &assoc->keys, key );
    array_add( &assoc->values, value );
    assoc->num_items++;
  }
  // Key exists! Replace value.
  else
  {
    // First remove key and value from array.
    array_remove_by_index( &(assoc->values), pos );
    array_remove_by_index( &(assoc->keys), pos );

    // Now add key and value but do not add counter
    // of num_items!
    array_add( &(assoc->keys), key );
    array_add( &(assoc->values), value );
  }
}

// ***************************************************
//  assoc_get
/*!
    @brief Get value from an assoc array

    @param a_assoc *assoc Array of values

    @param char *key Key what we need to get
*/
// ***************************************************
char *assoc_get( a_assoc *assoc, char *key )
{
  int i;
  char *retval = malloc(10);
  strcpy( retval, "" );
  
  // Loop all keys
  for( i=0; i < assoc->keys.num_items; i++ )
  {
    // Is this correct key?
    if( strcmp( assoc->keys.values[i], key ) == 0 )
    {
      // Count size of value of string in index i
      // and allocate enough space
      char *tmp = malloc( strlen( assoc->values.values[i] ) +1);
      if( tmp == NULL )
      {
        printf( "Malloc failed!\n" );
        exit(1);
      }

      // Return this value.
      free( retval );
      strcpy( tmp, assoc->values.values[i] );
      retval = tmp;

      // Do not continue searching anymore.
      break;
    }
  }

  return retval;
}

// ***************************************************
//  assoc_remove
/*!
    @brief Removes key-value pair from array

    @param a_assoc *assoc Array of values

    @param char *key Key what we want to remove
*/
// ***************************************************
void assoc_remove( a_assoc *assoc, char *key )
{
  // Search in which position array key is
  int pos = array_pos( &(assoc->keys), key );

  // There was key to remove. Remove key and value.
  if( pos != -1 )
  {
    array_remove_by_index( &(assoc->keys), pos );
    array_remove_by_index( &(assoc->values), pos );
    assoc->num_items--;
  }
}

// ***************************************************
//  assoc_keys
/*!
    @brief Get all possible keys from assoc array

    @param a_assoc *assoc Array of values

    @return a_array Array of keys
*/
// ***************************************************
a_array assoc_keys( a_assoc *assoc )
{
  a_array tmp = array_init();
  int i;

  for( i=0; i< assoc->num_items; i++ )
    array_add( &tmp, assoc->keys.values[i] );

  return tmp;
}
