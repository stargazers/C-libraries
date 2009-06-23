#ifndef __MY_WINDOW__
#define __MY_WINDOW__

GtkWidget *create_window( gchar *title, gboolean main_window );
gboolean delete_window( GtkWidget *window, GdkEvent *event, gpointer data );

#endif
