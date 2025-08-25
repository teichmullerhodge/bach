#include "state.h"

void cursor_pointer(GtkEventControllerMotion *motion, double x, double y,
                    gpointer udata) {
  (void)motion;
  (void)x;
  (void)y;
  GtkWidget *widget = GTK_WIDGET(udata);
  gtk_widget_set_cursor_from_name(widget, "pointer");
}

void cursor_default(GtkEventControllerMotion *motion, double x, double y,
                    gpointer udata) {
  (void)motion;
  (void)x;
  (void)y;

  GtkWidget *widget = GTK_WIDGET(udata);
  gtk_widget_set_cursor_from_name(widget, NULL);
}
