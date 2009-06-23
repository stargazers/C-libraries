#ifndef __MY_GTK__
#define __MY_GTK__

int create_thumbnail( char *file, char *new_file, int width, int height );
void generate_thumbnails( char *path, char *thumbs_path, char *prefix,
    int width, int height, int overwrite );
int get_image_size( char *image, int *width, int *height );

#define TN_OVERWRITE_ALWAYS 1
#define TN_OVERWRITE_IF_NEWER 2
#define TN_OVERWRITE_NEVER 4

#endif
