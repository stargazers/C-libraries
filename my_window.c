#include <gtk/gtk.h>
#include "my_window.h"

// ***************************************************
//  create_window
/*!
    @brief Create a new window

    @param gchar *title Title of a window

    @param gboolean main_window Is window main window or not.
           If true, automatically adds delete_event which
           causes gtk_main_quit() if users destroy window.

    @return GtkWidget Our window
*/
// ***************************************************
GtkWidget *create_window( gchar *title, gboolean main_window )
{
  GtkWidget *window;

  #ifdef DEBUG
    if( main_window )
      g_print( "Main window on %s\n\n", title );
  #endif

  // Luodaan uusi ikkuna
  window = gtk_window_new( GTK_WINDOW_TOPLEVEL );

  // Annetaan sille otsikko
  gtk_window_set_title( GTK_WINDOW( window ), title );

  // Lisätään destroy-eventin käsittelijä
  g_signal_connect( G_OBJECT( window ), "delete_event",
      G_CALLBACK( delete_window ), (gpointer) main_window );
    
  return window;
}


// ***************************************************
//  delete_window
/*!
    @brief Destroys normal window

    @param GtkWidget *window Window to destroy

    @param GdkEvent *event Event (destroy)

    @param gpointer data Extra data

    @return gboolean FALSE
*/
// ***************************************************
gboolean delete_window( GtkWidget *window, GdkEvent *event, gpointer data )
{
  #ifdef DEBUG
    g_print( "In deleteWindow...\n" );

    g_print( "Window title: %s\n\n", 
        gtk_window_get_title( GTK_WINDOW( window ) ) );
  #endif

  // Jos pääikkuna tuhottiin -> lopetetaan koko ohjelma
  if( ( gboolean ) data )
  {
      #ifdef DEBUG
        g_print( "Killing main window! Quitting...\n" );
      #endif

    gtk_main_quit();
    return FALSE;
  }
  else
  {
    // Pitää palauttaa false, jotta ikkunan saa tuhottua.
    return FALSE;
  }
}
