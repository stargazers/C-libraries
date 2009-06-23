#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
#include "my_array.h"
#include "my_file.h"
#include "my_curl.h"

// ***************************************************
//  get_urls
/*!
    @brief Get URLs from file

    @param a_array *urls Array where we save founded urls

    @param char *file File where we read

    @return int 0 = All OK
                -1 = File not found or can't read it
*/
// ***************************************************
int get_urls( a_array *urls, char *file )
{
  // Check if file exists
  if(! file_exists( file ) )
    return -1;

  // Open file and read its content
  char *data = file_get_contents( file, FT_TEXT );

  // How big our file was?
  int size = strlen( data );
  int i;

  // This is a dirty way... make a better one some day.
  for( i=0; i<size; i++ )
  {
    // If there is "http" text, then we have to search until we
    // found " char from our string.
    if( i>0 && data[i-1] == '"' && data[i] == 'h' && data[i+1] == 't'
        && data[i+2] == 't' && data[i+3] == 'p' )
    {
      char ch;

      // Start point from data-array.
      int counter = i;

      // Free memory to our string
      char *string = malloc( 1024 );
      int tmp_counter = 0;

      // Copy string until " is reached.
      while( ( ch = data[counter] ) != '"' )
      {
        // Copy char to string and add tmp_counter and counter values.
        string[tmp_counter] = ch;
        tmp_counter++;
        counter++;
      }

      // Add \0 in the end
      string[tmp_counter] = '\0';

      // Add our string to array
      array_add( urls, string );

      // Free memory of our string
      free( string );
    }
  }

  // Free data where is our file.
  free( data );

  return 0;
}

// ***************************************************
//  curl_download
/*!
    @brief Download a page to file

    @param char *url URL what we download

    @param char *filename File where we save results

    @return int 0 If all is ok
            -1 if cannot open file for writing
*/
// ***************************************************
int curl_download( char *url, char *filename )
{
  CURL *curl;
  CURLcode ret;
  FILE *file;

  file = fopen( filename, "w" );

  // Can't open file
  if(! file )
    return -1;

  curl = curl_easy_init();

  // Write data to file
  curl_easy_setopt( curl, CURLOPT_WRITEDATA, file );

  // Download this url
  curl_easy_setopt( curl, CURLOPT_URL, url );

  // Do it
  ret = curl_easy_perform( curl );
  curl_easy_cleanup( curl );

  fclose( file );

  return 0;
}
