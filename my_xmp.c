#include "/usr/include/exempi-2.0/exempi/xmp.h"
#include "/usr/include/exempi-2.0/exempi/xmpconsts.h"
#include "my_array.h"
#include "my_xmp.h"
#include <stdio.h>

// ***************************************************
//  xmp_bag_count_items
/*!
    @brief Count how many items is in XmpBag
    
    @param char *file File where we read

    @param const char namespace[] Namespace, listed
           in xmpconsts.h in libexempi

    @param char *tag Tag, for eg. "subject"

    @return int Number of items
*/
// ***************************************************
int xmp_bag_count_items( char *file, const char namespace[], char *tag )
{
  int i = 1;

  xmp_init();
  XmpStringPtr string = xmp_string_new();

  // Open file for writing
  XmpFilePtr tmpfile = xmp_files_open_new( file, XMP_OPEN_ONLYXMP );

  // Get pointer for xmp
  XmpPtr xmp = xmp_files_get_new_xmp( tmpfile );

  // Count items
  while( ( xmp_get_array_item( xmp, namespace, tag, i, string, NULL ) ) )
    i++;

  xmp_files_close( tmpfile, XMP_CLOSE_NOOPTION );
  xmp_files_free( tmpfile );
  xmp_string_free( string );
  xmp_free( xmp );

  return i -1;
}

// ***************************************************
//  xmp_delete_tag
/*!
    @brief Delete a XMP tag from file
    
    @param char *file File where we write

    @param const char namespace[] Namespace, listed
           in xmpconsts.h in libexempi

    @param char *tag Tag, for eg. "subject"
*/
// ***************************************************
void xmp_delete_tag( char *file, const char namespace[], char *tag )
{
  xmp_init();
  XmpFilePtr tmpfile = xmp_files_open_new( file, 
      XMP_OPEN_ONLYXMP | XMP_OPEN_FORUPDATE );

  XmpPtr xmp = xmp_files_get_new_xmp( tmpfile );

  if( xmp_tag_exists_in_file( file, namespace, tag ) )
  {
    xmp_delete_property( xmp, namespace, tag );
  }

  xmp_files_put_xmp( tmpfile, xmp );
  xmp_files_close( tmpfile, XMP_CLOSE_NOOPTION );
  xmp_files_free( tmpfile );
  xmp_free( xmp );
}

// ***************************************************
//  xmp_bag_tags_write_to_file
/*!
    @brief Writes multiple values to XmpBag. Function
           also checks if *tag is already in file, and
           if not, create it as a new XmpBag before trying
           to add any items to bag.
    
    @param char *file File where we write

    @param const char namespace[] Namespace, listed
           in xmpconsts.h in libexempi

    @param char *tag Tag, for eg. "subject"

    @param a_array *tags Tags what we want to add in bag
*/
// ***************************************************
void xmp_bag_tags_write_to_file( char *file, const char namespace[],
  char *tag, a_array *tags )
{
  // Check if there is XmpBag for our tags. If not, create it.
  if(! xmp_tag_exists_in_file( file, namespace, tag ) )
    xmp_tag_write_to_file( file, namespace, tag, "XmpBag" );

  // Always initialize XMP first
  xmp_init();

  // Count how many items we have to add
  int num_items = tags->num_items;
  int i = 0;

  // Open file for writing
  XmpFilePtr tmpfile = xmp_files_open_new( file, 
      XMP_OPEN_ONLYXMP | XMP_OPEN_FORUPDATE );

  // Get pointer for xmp
  XmpPtr xmp = xmp_files_get_new_xmp( tmpfile );

  // In this loop we add our array values to xmp pointer
  for( i=0; i < num_items; i++ )
    xmp_set_array_item( xmp, namespace, tag, i+1, tags->values[i], 0 );

  // Write values to file also
  xmp_files_put_xmp( tmpfile, xmp );

  // Close file and free memory
  xmp_files_close( tmpfile, XMP_CLOSE_NOOPTION );
  xmp_files_free( tmpfile );
  xmp_free( xmp );
}


// ***************************************************
//  xmp_tag_exists_in_file
/*!
    @brief Check if XMP tag exists in file
    
    @param char *file File where we read

    @param const char namespace[] Namespace, listed
           in xmpconsts.h in libexempi

    @param char *tag Tag, for eg. "subject"

    @return 1 = Tag exists, 0 = Tag does not exists.
*/
// ***************************************************
int xmp_tag_exists_in_file( char *file, const char namespace[], char *tag )
{
  int retval = 0;

  xmp_init();

  // Open file 
  XmpFilePtr tmpfile = xmp_files_open_new( file, XMP_OPEN_ONLYXMP );

  // Create new pointer to xmp
  XmpPtr xmp = xmp_files_get_new_xmp( tmpfile );

  XmpStringPtr string = xmp_string_new();

  // Is required tag in file?
  if( xmp_get_property( xmp, namespace, tag, string, NULL ) )
    retval = 1;

  xmp_files_close( tmpfile, XMP_CLOSE_NOOPTION );
  xmp_files_free( tmpfile );
  xmp_string_free( string );
  xmp_free( xmp );

  return retval;
}

// ***************************************************
//  xmp_tag_write_to_file
/*!
    @brief Add one XMP tag to file in namespace
    
    @param char *file File where we write

    @param const char namespace[] Namespace, listed
           in xmpconsts.h in libexempi

    @param char *tag Tag, for eg. "subject"

    @param char *value Value type, for eg. XmpBag
*/
// ***************************************************
void xmp_tag_write_to_file( char *file, const char namespace[],
    char *tag, char *value )
{
  xmp_init();

  XmpFilePtr f1 = xmp_files_open_new( file,
      XMP_OPEN_ONLYXMP | XMP_OPEN_FORUPDATE );

  XmpPtr xmp = xmp_files_get_new_xmp( f1 );

  xmp_set_property( xmp, namespace, tag, value, 0 );
  xmp_files_put_xmp( f1, xmp );
  xmp_files_close( f1, XMP_CLOSE_NOOPTION );
  xmp_files_free( f1 );
  xmp_free( xmp );
}

