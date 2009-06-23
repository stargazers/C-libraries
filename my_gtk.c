#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "my_array.h"
#include "my_file.h"
#include "my_string.h"
#include "my_gtk.h"

// ***************************************************
//  generate_thumbnails
/*!
    @brief Generate thumbnails from files in folder.
           If file already exists, it won't be overwrited.

    @param char *path Path where source files are

    @param char *thumbs_path Path where thumbnails
           are generated. NOTE! Directory must exists!

    @param char *prefix Prefix what will be added 
           in the beginning of thumbnail filename
    
    @param int width Thumbnail width

    @param int height Thumbnail height

    @param int overwrite Overwriting type
*/
// ***************************************************
void generate_thumbnails( char *path, char *thumbs_path, char *prefix,
    int width, int height, int overwrite )
{
  #ifdef MESSAGE
    g_print( "In function: generate_thumbnails\n" );
  #endif

  int num_files = 0, i = 0;

  // Initialize our array where we store filenames
  a_array files;
  files = array_init();

  // Check if we can create files
  if(! is_writeable( thumbs_path ) )
  {
    g_print( "Folder %s is not writeable!\n", thumbs_path );
    exit(1);
  }
  
  // Read files from folder and save number of files
  num_files = get_files_from_folder( path, &files, 0 );

  #ifdef MESSAGE
    g_print( "There is %i files in folder %s\n", num_files, path );
  #endif

  // Read file by file and create thumbnails
  for( i=0; i < num_files; i++ )
  {
    // Create full path and allocate memory for string
    char *full_path = NULL;
    char *thumb_file = NULL;

    full_path = str_concat( path, files.values[i] );
    thumb_file = str_concat( thumbs_path, prefix );
    thumb_file = str_concat( thumb_file, files.values[i] );

    // If we shouldn't overwrite files never, just skip this
    if( overwrite == TN_OVERWRITE_NEVER 
        && file_exists( thumb_file ) )
    {
      #ifdef MESSAGE
        g_print( "Overwrite type: NEVER OVERWRITE\n" );
        g_print( "File %s already exists, skipping.\n", thumb_file );
      #endif
    }
    // If thumb file exists, check its dimensions
    else if( overwrite == TN_OVERWRITE_IF_NEWER 
        && file_exists( thumb_file ) )
    {
      #ifdef MESSAGE
        g_print( "Overwrite type: OVERWRITE IF DIFFERENT SIZE REQUEST\n" );
      #endif

      int thumb_width, thumb_height;

      // Read old thumbnail image size
      get_image_size( thumb_file, &thumb_width, &thumb_height );

      if( thumb_width == width || thumb_height == height )
      {
        #ifdef MESSAGE
          g_print( "Old thumbfile is already at same size! " \
              "Skipping...\n" );
        #endif
      }
      else
      {
        #ifdef MESSAGE
          g_print( "Creating: %s from file %s\n", thumb_file,
              full_path );
        #endif
        create_thumbnail( full_path, thumb_file, width, height );
      }
    }
    // When we want to create thumbnail file
    else
    {
      #ifdef MESSAGE
      g_print( "Creating: %s from file %s\n", thumb_file,
          full_path );
      #endif
      create_thumbnail( full_path, thumb_file, width, height );
    }

    free( full_path );
    free( thumb_file );
  }

  array_free( &files );
}


// ***************************************************
//  create_thumbnail
/*!
    @brief Create thumbnail image from image

    @param char *file Original image filename

    @param char *new_file Thumbnail image filename

    @param int width Thumbnail width

    @param int height Thumbnail height

    @return 0 = All ok, -1 Can't open new_file for writing,
      -2 = Can't open file for reading as pixbuf
*/
// ***************************************************
int create_thumbnail( char *file, char *new_file, int width, int height )
{
  #ifdef MESSAGE
    printf( "IN FUNCTION: create_thumbnail\n" );
    printf( "Source file: %s\n", file );
    printf( "Target file: %s\n", new_file );
  #endif

  GdkPixbuf *pixbuf;
  GError *error = NULL;
  FILE *fp;

  // Try to open file for writing
  fp = fopen( new_file, "w" );
  if( fp == NULL )
  {
    #ifdef DEBUG
    printf( "Can\'t open file %s for writing!\n\n", new_file );
    #endif
    return -1;
  }
  fclose( fp );

  // Open original image at size width * height
  pixbuf = gdk_pixbuf_new_from_file_at_size( file, width, height, 
      &error );

  // If it fails, we should take care of it
  if( pixbuf == NULL )
  {
    #ifdef DEBUG
    printf( "Failed to create pixbuf from file %s!\n", file );
    return -2;
    #endif
  }

  // Save our pixbuf to thumbnail file 
  gdk_pixbuf_save( pixbuf, new_file, "jpeg", &error,
      "quality", "75", NULL );

  #ifdef MESSAGE
    printf( "Thumbnail created succesfully!\n\n" );
  #endif

  return 0;
}

// ***************************************************
//  get_image_size
/*!
    @brief Read image size

    @param char *image File

    @param int *width Pointer to width variable

    @param int *height Pointer to height variable

    @return 0 = All ok, -1 = File not found, 
            -2 = Can't load file to pixbuf
*/
// ***************************************************
int get_image_size( char *image, int *width, int *height )
{
  #ifdef MESSAGE
    g_print( "\n======================================\n" );
    g_print( "In function: get_image_size\n" );
    g_print( "======================================\n" );
    g_print( "File: %s\n", image );
  #endif

  // If file not found, return -1
  if(! file_exists( image ) )
  {
    #ifdef DEBUG
    g_print( "File %s not found!\n", image );
    #endif
    return -1;
  }

  GdkPixbuf *pixbuf;
  GError *error = NULL;

  // Load image file to pixbuf
  pixbuf = gdk_pixbuf_new_from_file( image, &error );

  // Oh crap, it failed... Return -2
  if( pixbuf == NULL )
  {
    #ifdef DEBUG
    g_print( "File %s cannot be loaded as pixbuf!\n\n", image );
    #endif
    return -2;
  }

  // Save width and height to pointers
  *width = gdk_pixbuf_get_width( pixbuf );
  *height = gdk_pixbuf_get_height( pixbuf );

  #ifdef MESSAGE
    g_print( "Image width: %i\n", *width );
    g_print( "Image height: %i\n\n", *height );
  #endif

  // Remember to free allocated memory!
  g_object_unref( pixbuf );
  return 0;
}
