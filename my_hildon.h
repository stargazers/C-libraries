#ifndef __MY_HILDON__
#define __MY_HILDON__

#include <gtk/gtk.h>

// Windows states
#define WS_NORMAL 0
#define WS_FULLSCREEN 1

//! Window width in fullscreen
#define WS_FS_WIDTH 800

//! Window height in fullscreen
#define WS_FS_HEIGHT 480

//! Window width in windowed
#define WS_NORMAL_WIDTH 696

//! Window height in windowed
#define WS_NORMAL_HEIGHT 396

typedef struct
{
  //! Window
  GtkWidget *window;

  //! Window state. WS_FULLSCREEN or WS_NORMAL.
  int window_state;

  //! Current width 
  int width;

  //! Current height
  int height;

} a_window;

a_window *create_window( char *title );
/*
void key_pressed( GtkWidget *window, GdkEvent *event,
    gpointer *window_struct );
*/

#endif
