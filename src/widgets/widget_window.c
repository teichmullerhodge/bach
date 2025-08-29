#include "widget_window.h"

GtkWidget *widget_window(GtkApplication *app, const char *title, i32 width,
                         i32 height) {
  GtkWidget *window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), title);
  gtk_window_set_default_size(GTK_WINDOW(window), width, height);

  return window;
}
