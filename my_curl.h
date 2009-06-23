#ifndef __MY_CURL__
#define __MY_CURL__

#include "my_array.h"

int curl_download( char *url, char *filename );
int get_urls( a_array *urls, char *file );

#endif
