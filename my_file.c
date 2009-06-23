#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "my_array.h"
#include "my_string.h"
#include "my_file.h"

// ***************************************************
//  get_files_recursive
/*!
    @brief Read folder contents recursively. This is
           only a shorhand for get_files_from_folder_recursive
           which needs also extra parameter folders.

    @param char *folder Folder where we start

    @param a_array *all_files Array where we store filenames
           with path.
*/
// ***************************************************
void get_files_recursive( char *folder, a_array *all_files )
{
  a_array folders;
  folders = array_init();
  get_files_from_folder_recursive( folder, &folders, &(*all_files) );
  array_free( &folders );
}

// ***************************************************
//  get_files_from_folder_recursive
/*!
    @brief Reads folders contentes recursively. This is
           not meant to call directly, use get_files_recursive
           instead!

    @param char *folder Folder where we start

    @param a_array *folders Folders what are found inside
           of folder.

    @param a_array *all_files Array where we store filenames
           with path.
*/
// ***************************************************
void get_files_from_folder_recursive( char *folder, a_array *folders, 
  a_array *all_files )
{
  int i=0;
  size_t new_size;

  // Subfolders of folder
  a_array tmp_folders;
  tmp_folders = array_init();

  // Get subfolders and remove . and ..
  get_folders_from_folder( folder, &tmp_folders, 0 );
  array_remove( &tmp_folders, "." );
  array_remove( &tmp_folders, ".." );

  // Loop all folders what we have found
  for( i=0; i < tmp_folders.num_items; i++ )
  {
    // Allocate memory for full path, what is folder name
    // length + 1 for \0
    char *full_path = malloc( strlen( folder ) + 1 );

    if( full_path == NULL )
    {
      printf( "Allocation failed!\n" );
      exit(1);
    }

    // Copy folder name to full_path
    strcpy( full_path, folder );

    // Last character of string.
    char *last_char = malloc( 2 );

    if( last_char == NULL )
    {
      printf( "MALLOC FAILS IN recursive!\n" );
      exit(1);
    }

    // Copy last character of path to last_char
    strcpy( last_char, &full_path[ strlen( full_path ) -1 ] );

    // Count required space for string 
    new_size = strlen( full_path ) + strlen( tmp_folders.values[i] ) + 2;

    // If there is already / in the string end, new_size is 
    // then one byte smaller
    if( strcmp( last_char, "/" ) == 0 )
      new_size--;

    // Allocate memory
    char *tmp = malloc( new_size );

    if( tmp == NULL )
    {
      printf( "Memory allocation failed!\n" );
      exit(1);
    }

    // Add / in the end of path
    if( strcmp( last_char, "/" ) != 0 )
    {
      strcpy( tmp, full_path );
      strcat( tmp, "/" );
      free( full_path );
      full_path = tmp;
    }
    // There was already /, no need to add it.
    else
    {
      strcpy( tmp, full_path );
      free( full_path );
      full_path = tmp;
    }

    // Add folder name to full_path
    strcat( full_path, tmp_folders.values[i] );

    // Call this function with those folders we have found here
    get_files_from_folder_recursive( full_path, folders, all_files );

    // Free allocations of memory
    free( last_char );
    free( full_path );
  }

  // Remember to free temp folders!
  array_free( &tmp_folders );

  // Now we have to get files from this folder.
  // Last parameter means that we have to add full pathname
  // to array items.
  get_files_from_folder( folder, all_files, 1 );
}

// ***************************************************
//  get_file_extension
/*!
    @brief Get file extension. 

    @param char *filename Filename where we read

    @return char * File extension without leading dot.
*/
// ***************************************************
char *get_file_extension( char *filename )
{
  // Allocate memory for extension.
  char *extension = malloc( strlen( filename ) + 1 );

  if( extension == NULL )
  {
    printf( "Failed to malloc!\n" );
    exit(1);
  }

  // Our array item
  a_array tmp;
  tmp = array_init();

  // Explode will split filename to array items
  explode( ".", filename, &tmp );

  // Copy last item of array to tmp
  strcpy( extension, tmp.values[tmp.num_items-1] );
  array_free( &tmp );

  return extension;
}

// ***************************************************
//  file_exists
/*!
    @brief Checks if file exists

    @param char *file File to check

    @return int 1 = File exists
                0 = File does not exists
*/
// ***************************************************
int file_exists( char *file )
{
  #ifdef DEBUG
    printf("\n==========================================\n" );
    printf( "IN FUNCTION:\t file_exists\n" );
    printf( "==========================================\n" );
    printf( "Check if file exists: %s\n", file );
  #endif

  FILE *fp;
  fp = fopen( file, "r" );

  #ifdef DEBUG
    if(! fp )
      printf( "File does not exists.\n" );
    else
      printf( "File exists.\n" );
  #endif

  if(! fp )
  {
    return 0;
  }
  else
  {
    fclose( fp );
    return 1;
  }
}

// ***************************************************
//  is_writeable
/*!
    @brief Checks if folder is writeable

    @param char *path Folder that we must check

    @return int 1 = Files can be written in this folder
                0 = Files cannot be written in this folder
*/
// ***************************************************
int is_writeable( char *path )
{
  #ifdef DEBUG
    printf("\n==========================================\n" );
    printf( "IN FUNCTION:\t is_writeable\n" );
    printf( "==========================================\n" );
    printf( "Check if folder is writeable: %s\n", path );
  #endif

  FILE *fp;
  char *tmp;

  // Allocate memory to string
  tmp = malloc( strlen( path ) + strlen( "dummyfile" ) + 1 );

  // Malloc fails
  if( tmp == NULL )
  {
    printf( "Malloc fails in is_writeable!\n" );
    exit(1);
  }

  // Copy path to string tmp
  strcpy( tmp, path );

  // Add "dummyfile" in the end of the tmp
  strcat( tmp, "dummyfile" );

  // Try to open path/dummyfile for writing
  fp = fopen( tmp, "w" );

  // Failed to create dummyfile
  if(! fp )
  {
    #ifdef DEBUG
      printf( "Folder is unwriteable.\n" );
    #endif
    return 0;
  }

  // Created dummyfile ok! Delete it now.
  remove( tmp );

  #ifdef DEBUG
    printf( "Folder is writeable.\n" );
  #endif
  return 1;
}

// ***************************************************
//  get_files_from_folder
/*!
    @brief Read folder contents

    @param char *folder Folder where we search 

    @param a_array *filenames Array where we can add found
           filenames

    @return int Number of found files
*/
// ***************************************************
int get_files_from_folder( char *folder, a_array *filenames, 
  int add_path )
{
  #ifdef DEBUG
    printf("\n==========================================\n" );
    printf( "IN FUNCTION:\t get_files_from_folder\n" );
    printf( "==========================================\n" );
    printf( "Read files from folder %s\n", folder );
  #endif

  int num_files = 0;
  num_files = read_folder_content( folder, &(*filenames), DT_REG, 
    add_path );

  return num_files;
}

// ***************************************************
//  get_folders_from_folder
/*!
    @brief Read subfolders of folder

    @param char *folder Folder where we search 

    @param a_array *folders Array where we can add found
           subfolders

    @return int Number of found folders
*/
// ***************************************************
int get_folders_from_folder( char *folder, a_array *folders,
  int add_path )
{
  #ifdef DEBUG
    printf("\n==========================================\n" );
    printf( "IN FUNCTION:\t get_folders_from_folder\n" );
    printf( "==========================================\n" );
    printf( "Read folders from folder %s\n", folder );
  #endif

  int num_folders = 0;
  num_folders = read_folder_content( folder, &(*folders), 
    DT_DIR, add_path );

  return num_folders;
}

// ***************************************************
//  read_folder_content
/*!
    @brief Read all files/folders/whatever from folder

    @param char *folder Folder where we search 

    @param a_array *content Array where we can add found
           values

    @param unsigned char type Type. Same types than dirent.h
           types. For eg. DT_REG, DT_DIR etc.

    @param int add_path Add folder in filename or not. 1 = add, 0 = Not.

    @return int Number of found itens
*/
// ***************************************************
int read_folder_content( char *folder, a_array *content, 
    unsigned char type, int add_path )
{
  #ifdef DEBUG
    printf("\n==========================================\n" );
    printf( "IN FUNCTION:\t read_folder_content\n" );
    printf( "==========================================\n" );
    printf( "Folder where we read: %s\n", folder );
  #endif

  struct dirent *de = NULL;
  DIR *d = NULL;
  int num_files = 0;

  // Try to open directory
  d = opendir( folder );

  // We can't read that folder!
  if( d == NULL )
  {
    printf( "Failed to open folder %s!\n", folder );
  }
  // We can read this folder. Great. Then try to read files.
  else
  {
    // Read all the files from the folder and store them in
    // our tmp array. Also, save number of files in variable.
    while( ( de = readdir( d ) ) )
    {
      #ifdef DEBUG
        printf( "Current item: %s\n", de->d_name );
        printf( "de->d_type is %i, required %i\n", de->d_type, type );
      #endif

      if( de->d_type == type || de->d_type == 0 )
      {
        char *tmp = NULL;

        num_files++;

        // Should we add path before filename?
        if( add_path == 1 )
        {
          // Last character of foldername.
          char *last_char = malloc( 2 );

          // Copy last character of folder name to last_char
          strcpy( last_char, &folder[ strlen( folder ) -1 ] );

          size_t new_size;

          // Slash already in pathname
          new_size = strlen( folder ) + strlen( de->d_name ) + 1;

          if( strcmp( last_char, "/" ) != 0 )
            new_size++;

          tmp = malloc( new_size );

          if( tmp == NULL )
          {
            printf( "Allocation of memory failed\n" );
            exit(1);
          }

          // Is last char /
          if( strcmp( last_char, "/" ) == 0 )
          {
            strcpy( tmp, folder );
            strcat( tmp, de->d_name );
          }
          else
          {
            strcpy( tmp, folder );
            strcat( tmp, "/" );
            strcat( tmp, de->d_name );
          }

          free( last_char );
        }

        // Don't add path!
        else
        {
          tmp = malloc( strlen(de->d_name ) + 1 );
          strcpy( tmp, de->d_name );
        }

        // Add our value to our array
        array_add( &(*content), tmp );

        // Remember to free allocated memory!
        free( tmp );
      }
    }
  }
  closedir( d );

  return num_files;
}

// ***************************************************
//  basename
/*!
    @brief Get a basename of file

    @param char *file File with full path

    @retrun char * Filename
*/
// ***************************************************
char *basename( char *file )
{
  a_array tmp;
  tmp = array_init();

  // Explode string using / as delimiter. Save pieces
  // in array tmp.
  explode( "/", file, &tmp );

  // Allocate enough memory for our string.
  char *basename;
  basename = malloc( strlen( tmp.values[ tmp.num_items -1 ] ) +1 );

  if( basename == NULL )
  {
    printf( "Failed to malloc!\n" );
    exit(1);
  }

  // Basename is last item of our array. 
  strcpy( basename, tmp.values[ tmp.num_items -1 ] );

  // Remember to free array memory!
  array_free( &tmp );

  return basename;
}


// ***************************************************
//  filter_by_extension
/*!
    @brief Filter filenames by extension

    @param a_array *filenames Filenames

    @param a_array extensions File extensions to include
*/
// ***************************************************
void filter_by_extension( a_array *filenames, a_array extensions )
{
  int i;
  a_array tmp;
  tmp = array_init();

  // Loop through all files and search if file extension
  // is what we wanted. If it is, add it to array tmp.
  for( i=0; i < filenames->num_items; i++ )
  {
    char *ext;

    ext = get_file_extension( filenames->values[i] );

    // If that extension must be added to array tmp
    if( in_array( &extensions, ext ) )
      array_add( &tmp, filenames->values[i] );

    free( ext );
  }

  // Free old array
  array_free( filenames );

  // Now we must point our array to this new array.
  *filenames = tmp;
}

// ***************************************************
//  get_file_size
/*!
    @brief Get size of file in bytes

    @param char *file File to read

    @return unsigned long Size in bytes,
      -1 if file cannot be read
*/
// ***************************************************
unsigned long get_file_size( char *file )
{
  if(! file_exists( file ) )
    return -1;

  FILE *fp;
  fp = fopen( file, "rb" );

  // Some other reason than file does not exists. Still
  // we can't read it. :(
  if( fp == NULL )
  {
    fclose(fp);
    return -1;
  }

  unsigned long filesize = 0;

  // Seek to the end of file
  fseek( fp, 0, SEEK_END );

  // What is our position in file pointer?
  // That is the length of our file.
  filesize = ftell( fp );

  fclose( fp );

  return filesize;
}

// ***************************************************
//  file_get_contents
/*!
    @brief Get file contents to buffer

    @param char *file File what we read

    @param int mode FT_TEXT add string terminator in the end, 
               FT_BINARY does not add it.

    @return char * Readed data. Remember to free it!
*/
// ***************************************************
char *file_get_contents( char *file, int mode )
{
	// We must check if file really exists!
	// If not, then just return empty string.
	if(! file_exists( file ) )
	{
		char *buffer = malloc( 2 );
		strcpy( buffer, '\0' );
		return buffer;
	}

  size_t new_size;
  FILE *fp;
  fp = fopen( file, "rb" );

  // Get file size in bytes
  unsigned long filesize;
  filesize = get_file_size( file );

  // Allocate enough space for data
  if( mode == FT_BINARY )
    new_size = filesize;
  else
    new_size = filesize +1;

  // Allocate data
  char *buffer = malloc( new_size );

  if( buffer == NULL )
  {
    printf( "Mallocation failed!\n" );
    exit(1);
  }

  // Read file to buffer
  fread( buffer, sizeof(char), filesize, fp );
  fclose( fp );

  if( mode == FT_TEXT )
    buffer[filesize] = '\0';

  return buffer;
}

// ***************************************************
//  get_filename_without_extension
/*!
    @brief Get filename without its extension

    @param char *filename Filename

    @return char * Filename without extension
*/
// ***************************************************
char *get_filename_without_extension( char *filename )
{
  a_array tmp;
  tmp = array_init();

  char *base = basename( filename );
  explode( ".", base, &tmp );
 
  char *retval;
  retval = malloc( strlen( tmp.values[0] ) );

  if( retval == NULL )
    exit(1);

  strcpy( retval, tmp.values[0] );

  array_free( &tmp );

  return retval;
}
