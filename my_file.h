#ifndef __MY_FILE__
#define __MY_FILE__

#include "my_array.h"
#define FT_TEXT 0
#define FT_BINARY 1

int file_exists( char *file );
int is_writeable( char *path );
int get_files_from_folder( char *folder, a_array *filenames, 
  int add_path );
int get_folders_from_folder( char *folder, a_array *folders,
  int add_path );
int read_folder_content( char *folder, a_array *content, 
    unsigned char type, int add_path );
char* get_file_extension( char *filename );
void get_files_from_folder_recursive( char *folder, a_array *folders, 
  a_array *all_files );
void get_files_recursive( char *folder, a_array *all_files );
char *basename( char *file );
void filter_by_extension( a_array *filenames, a_array extensions );
unsigned long get_file_size( char *file );
char *file_get_contents( char *file, int mode );
char *get_filename_without_extension( char *filename );

#endif 
