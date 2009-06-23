#include <stdio.h>
#include <stdlib.h>
#include "my_debug.h"

// ***************************************************
//  malloc_failed
/*!
    @brief This function is for printing error message
           because malloc have failed, and then exit.
*/
// ***************************************************
void malloc_failed( void )
{
  printf( "Failed to allocate memory!\n" );
  exit(1);
}

// ***************************************************
//  debug_print
/*!
    @brief This function is for printing debugging
           messages and notices etc.
           
    @param int type Type. Check DP_TYPE.
*/
// ***************************************************
void debug_print( int type, char *text )
{
  switch( type )
  {
    // DEBUG
    case DP_DEBUG:
      #ifdef DEBUG
        printf( "%s\n", text );
      #endif
      break;

    case DP_MESSAGE:
      #ifdef MESSAGE
        printf( "%s\n", text );
      #endif
      break;
  }

}
