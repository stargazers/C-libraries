#ifndef __MY_STRING__
#define __MY_STRING__

#include "my_array.h"

char *str_concat( char *str, char *str2 );
void explode( char *limit, char *value, a_array *array );
char *implode( char *limit, a_array *array );
int str_count( char *data, char value );
char *getline( char *data, int linenum );
int strpos( char *string, char value, int offset );
char *substr( char *string, int start, int length );
char *get_numbers_from_string( char *string );

#endif
