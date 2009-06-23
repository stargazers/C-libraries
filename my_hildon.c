#include <hildon/hildon.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include "my_hildon.h"
#include "my_debug.h"

static void destroy_window( GtkWidget *window, GdkEvent *event, 
    gpointer *data )
{
  gtk_main_quit();
}

// ***************************************************
//  create_window
/*!
    @brief Creates new Hildon window and add a signal
           that allows closing window. Also, we add
           possibility to change fullscreen and back.
    
    @param char *title Window title.

    @return *a_window Pointer to window structre.
            Note! This must be freed!
*/
// ***************************************************
a_window *create_window( char *title )
{
  HildonProgram *program;
  GtkWidget *window;

  // Allocate memory for our window structure from heap
  a_window *tmp = malloc( sizeof( a_window ) );

  if( tmp == NULL )
    malloc_failed();

  /* Create new hildon program */
  program = HILDON_PROGRAM( hildon_program_get_instance() );

  /* Set title of our program */
  g_set_application_name( title );

  /* Create new window */
  window = hildon_window_new();

  /* Add window to program */
  hildon_program_add_window( program, HILDON_WINDOW( window ) );

  /* Save values to structure a_window. */
  tmp->window = window;
  tmp->window_state = WS_NORMAL;
  tmp->width = 796;
  tmp->height = 396;

  /* When user closes this window, we must quit. */
  g_signal_connect( G_OBJECT( tmp->window ), "delete_event",
      G_CALLBACK( destroy_window ), NULL );
  /*
  g_signal_connect( G_OBJECT( tmp->window ), "key_press_event",
      G_CALLBACK( key_pressed ), tmp );
  */
  /* Return our structure. */
  return tmp;
}
