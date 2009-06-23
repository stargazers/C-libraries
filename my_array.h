#ifndef __MY_ARRAY__
#define __MY_ARRAY__

#include <stdio.h>
#include <stdlib.h>

//! My own array structure
typedef struct
{
  //! Number of items in array
  int num_items;

  //! How much we have allocated space for values
  size_t allocated;

  //! How much we have used space of our allocated space
  size_t used;

  //! Values of array
  char **values;
} a_array;

a_array array_init( void );
void array_free( a_array *array );
void array_add( a_array *array, char *value );
int in_array( a_array *array, char *value );
void array_remove( a_array *array, char *value );
int array_pos( a_array *array, char *value );
void array_remove_by_index( a_array *array, int index );
#endif
