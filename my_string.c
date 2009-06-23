#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "my_array.h"
#include "my_string.h"

// ***************************************************
//  str_concat
/*!
    @brief Concanates two char* together

    @param char *str String 1

    @param char *str2 String 2

    @return char * Concantenated string
*/
// ***************************************************
char *str_concat( char *str, char *str2 )
{
  char *tmp = NULL;

  tmp = malloc( strlen( str ) + strlen( str2 ) + 1);

  if( tmp == NULL )
  {
    printf( "Malloc fails in str_concat!\n" );
    exit(1);
  }

  strcpy( tmp, str );
  strcat( tmp, str2 );

  return tmp;
}

// ***************************************************
//  explode
/*!
    @brief Split a string to smaller chunks in array

    @param char *limit Limiter char

    @param char *value String itself

    @return a_array *array Array where we add items
*/
// ***************************************************
void explode( char *limit, char *value, a_array *array )
{
  // Create new pointer and copy value to it.
  // This is required, because strtok will change the 
  // value itself!
  char *tmp = malloc( strlen( value ) + 1 );
  strcpy( tmp, value );

  char *token;

  // Read first token
  token = strtok( tmp, limit );

  // Do tokenizing until NULL is reached
  while( token != NULL )
  {
    array_add( &(*array), token );
    token = strtok( NULL, limit );
  }

  free( tmp );
}


// ***************************************************
//  implode
/*!
    @brief Concatenate array items to one string

    @param char *limit String which come between elements

    @param a_array *array Array where items are

    @return char * Concatenated string

    @include implode.c
*/
// ***************************************************
char *implode( char *limit, a_array *array )
{
  // How many items is in array
  int num_items = array->num_items;

  int i=0;
  char *tmp = "";
  char *final = "";

  // Add all strings to final string
  for( i=0; i < num_items; i++ )
  {
    // Add current array element to tmp
    tmp = str_concat( final, array->values[i] );

    // Free the memory. Don't do this if this is first time in loop!
    if( i > 0 )
      free( final );

    // Add limit string to final. 
    // Don't do this in last time when we loop this for-loop!
    if( i < num_items -1 )
      final = str_concat( tmp, limit );
    else
      final = str_concat( tmp, "" );

    // Free memory from tmp string
    free( tmp );
  }

  // Return final string
  return final;
}

// ***************************************************
//  str_count
/*!
    @brief Count number of wanted characters from data

    @param char *data Data where we search

    @param char value Value what to search

    @return Number of found values in data.
*/
// ***************************************************
int str_count( char *data, char value )
{
  size_t max, i, found = 0;

  // How big our data char-array is?
  max = strlen( data );

  // Loop the whole data and count how 
  // many values we found
  for( i=0; i < max; i++ )
  {
    char ch = data[i];

    if( ch == value )
      found++;
  }

  return found;
}

// ***************************************************
//  getline
/*!
    @brief Read line from data. First line is number 1!

    @param char *data Data where we read

    @param int linenum What line we want to get

    @return char *Line
*/
// ***************************************************
char *getline( char *data, int linenum )
{
  // We want to count minus one, because lines starts from zero
  // and users think that first line is number 1, not zero...
  linenum--;

  int i2 = 0, i;

  // Read data until we have found the correct line
  for( i=0; i<linenum; i++ )
  {
    while( ( data[i2] != '\n' ) )
      i2++;

    i2++;
  }

  // Correct line is next line
  i2++;

  // Save position where our line starts in data.
  int pos = i2;

  // Count length of this line
  while( ( data[i2] != '\n' ) )
    i2++;

  // Now we can count strlen of our line. It is current 
  // position minus position of start of string.
  int length = ( i2 - pos ) +2;

  // Allocate memory for it
  char *tmp = malloc( length );

  if( tmp == NULL )
  {
    printf( "Mallocation failed\n" );
    exit(1);
  }

  // Counter is array index number to new string
  int counter=0;

  // Read line and add it to array tmp
  for( i=pos-1; i < i2; i++ )
  {
    tmp[counter] = data[i];
    counter++;
  }

  // Our string must be NULL-terminated string.
  tmp[counter] = '\0';

  return tmp;
}

// ***************************************************
//  strpos
/*!
    @brief Find position of char in string.

    @param char *string String where we search

    @param char value Char to find

    @param int offset Position where we start searching.

    @return int Greater than zero is a position of char
            in string, -1 if there is no char in string.
*/
// ***************************************************
int strpos( char *string, char value, int offset )
{
  int i, length=0, found=-2;

  length = strlen( string );

  for( i=offset; i < length; i++ )
  {
    if( string[i] == value )
    {
      found = i;
      break;
    }
  }

  return found+1;
}

// ***************************************************
//  substr
/*!
    @brief Returns a part of a string

    @param char *string String where we search

    @param int start Start position

    @param int length End position
    
    @retrun char * Substring
*/
// ***************************************************
char *substr( char *string, int start, int length )
{
  int counter = 0;
  int i;

  // How long our new size will be
  size_t new_size = ( length - start ) +1;

  // Allocate memory for return string
  char *tmp = malloc( new_size );

  if( tmp == NULL )
  {
    printf( "Malloc failed!\n" );
    exit(1);
  }

  // Add charcters from start to length to our string
  for( i=start; i < length-1; i++ )
  {
    tmp[counter] = string[i];
    counter++;
  }

  // String must be NULL-terminated!
  tmp[counter] = '\0';

  return tmp;
}

char *get_numbers_from_string( char *string )
{
  // Here we store numbers from string as string
  char *final = malloc( strlen( string ) + 1 );
  strcpy( final, "" );
  int max = strlen( string );
  int i;

  // Loop all chars from string and
  // try to find numbers from it.
  for( i=0; i < max; i++ )
  {
    char ch[2] = { string[i], '\0' };
    int i2;

    for( i2=0; i2 < 9; i2++ )
    {
      char *val = malloc( 2 );
      sprintf( val, "%i", i2 );

      if( strcmp( ch, val ) == 0 )
      {
        strcat( final, ch );
        break;
      }
    }
  }

  return final;
}

// ***************************************************
//  fix_chars
/*!
    @brief Fix characters from hexadecimal values
           to normal characters.

    @param char *src Source where values include
           hexadecimal values.

    @param char *dst Destionation, where we put
           fixed string.
*/
// ***************************************************
void fix_chars( char *src, char *dst )
{
  // Loop this until we reach the end of our source string.
  while( *src != '\0' )
  {
    // Key code
    int code;

    // In this switch-case we change converted hex values back
    // to correct characters, and '+ chars to space.
    switch( *src )
    {
      // Char + must be converted to space.
      case '+':
        *dst = ' ';
        break;

      // Hexadecimal value!
      case '%':

        // Convert hexadecimal character values back to correct values
        if( sscanf( src+1, "%2x", &code ) != 1 ) 
          code = '?';

        *dst = code;

        // Move two chars, because every hexadecimal values are
        // stored using two characters after % char.
        src += 2;
        break;

      // Normally we can just copy value from src to dst
      default:
        *dst = *src;
        break;
    }

    // Move to next char in both char arrays.
    src++;
    dst++;
  }

  // In the end we must add \n\0 in dest, so string will be
  // ended correctly...
  *dst = '\n';
  *++dst = '\0';
}
