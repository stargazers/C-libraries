#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_array.h"

// ***************************************************
//  array_init
/*!
    @brief Initialize array

    @return a_array Empty array
*/
// ***************************************************
a_array array_init( void )
{
  // Our new array
  a_array tmp;

  // Allocate space for 10 items
  size_t new_size = sizeof( char * ) * 10;

  // Number of items in array
  tmp.num_items = 0;

  // Allocate memory for our array
  tmp.values = malloc( new_size );

  // How much memory is used.
  tmp.used = 0;

  // How much we have allocated memory.
  tmp.allocated = new_size;

  return tmp;
}

// ***************************************************
//  array_remove
/*!
    @brief Remove string from array

    @param a_array *array Array of strings

    @param char *value Value to remove from array
*/
// ***************************************************
void array_remove( a_array *array, char *value )
{
  int i;
  a_array tmp;
  tmp = array_init();

  // Copy all items to new array except the value which
  // is same than *value.
  for( i=0; i < array->num_items; i++ )
  {
    if( strcmp( array->values[i], value ) != 0 )
      array_add( &tmp, array->values[i] );
  }

  // Remember to free old array memory!
  array_free( array );

  // Our array must point to new array.
  *array = tmp;
}

// ***************************************************
//  array_free
/*!
    @brief Frees array memory 

    @param a_array Array which items we want free
*/
// ***************************************************
void array_free( a_array *array )
{
  int i;

  // Free all memory what we have allocated to our strings
  for( i=0; i < array->num_items; i++ )
    free( array->values[i] );

  // And also free array values char**
  free( array->values );
}

// ***************************************************
//  array_add
/*!
    @brief Add an item to array

    @param a_array *array Our array where we add item

    @param char *value Value to add
*/
// ***************************************************
void array_add( a_array *array, char *value )
{
  // How much there should be memory for our array items?
  // It is number of array items + one * size of one char *.
  size_t required = sizeof( char * ) * ( array->num_items +1 );

  // If we need more memory than we have allocated, then
  // we have to realloc.
  if( array->allocated < required )
  {
    // Reallocate. Do new size 2x bigger than it was last time.
    char **tmp = realloc( &(*array->values), (required * 2 ) );

    // If we failed here, we must exit.
    if( tmp == NULL )
    {
      printf( "Failed to realloc in array_add!\n" );
      exit( 1 );
    }

    // Remember to update required informations
    array->allocated = required * 2;

    // Use correct pointer
    array->values = tmp;
  }

  // Allocate memory for our string
  array->values[ array->num_items ] = malloc( strlen( value ) + 1 );

  // Copy our string to our allocated memory block
  strcpy( array->values[array->num_items], value );

  // Add item counter
  array->num_items++;

  // Update info how much we have used memory
  array->used = array->num_items * sizeof( char * );
}

// ***************************************************
//  in_array
/*!
    @brief Check if value exists in array

    @param a_array array Array where we search

    @param char *value What value we are looking for

    @return 1 = Value is in array, 0 = Value is not in array
*/
// ***************************************************
int in_array( a_array *array, char *value )
{
  int i;

  for( i=0; i < array->num_items; i++ )
  {
    // Found!
    if( strcmp( array->values[i], value ) == 0 )
      return 1;
  }

  // Not found
  return 0;
}

// ***************************************************
//  array_pos
/*!
    @brief Return item index number in array.

    @param a_array *array Array where we search

    @param char *value Value to search

    @return int -1 if there is no value in array,
            0 or greater elsewhere.
*/
// ***************************************************
int array_pos( a_array *array, char *value )
{
  int i;

  // Loop all array items and search value
  for( i=0; i < array->num_items; i++ )
  {
    if( strcmp( array->values[i], value ) == 0 )
      return i;
  }

  return -1;
}

// ***************************************************
//  array_remove_by_index
/*!
    @brief Removes Nth item from the array

    @param a_array *array Array where we search

    @param int index Index number of value what we want
           to remove.
*/
// ***************************************************
void array_remove_by_index( a_array *array, int index )
{
  a_array tmp = array_init();

  int i;
  for( i=0; i < array->num_items; i++ )
  {
    if( i != index )
      array_add( &tmp, array->values[i] );
  }

  array_free( array );
  *array = tmp;
}
