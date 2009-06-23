#ifndef __MY_XMP__
#define __MY_XMP__

void xmp_bag_tags_write_to_file( char *file, const char namespace[],
  char *tag, a_array *tags );

int xmp_tag_exists_in_file( char *file, const char namespace[], 
  char *tag );

void xmp_tag_write_to_file( char *file, const char namespace[],
    char *tag, char *value );

void xmp_delete_tag( char *file, const char namespace[], char *tag );

int xmp_bag_count_items( char *file, const char namespace[], char *tag );


#endif
