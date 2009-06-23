#ifndef __my_assoc__
#define __my_assoc__

//! Assoc array structure
typedef struct
{
  //! Assoc array keys
  a_array keys;

  //! Assoc array values
  a_array values;

  //! Number of values in array
  int num_items;
} a_assoc;

a_assoc assoc_init();
void assoc_free( a_assoc *assoc );
void assoc_add( a_assoc *assoc, char *key, char *value );
char *assoc_get( a_assoc *assoc, char *key );
void assoc_remove( a_assoc *assoc, char *key );
a_array assoc_keys( a_assoc *assoc );

#endif
