#ifndef STATE_H
#define STATE_H

#include <gtk/gtk.h>

void cursor_pointer(GtkEventControllerMotion *button, double x, double y,
                    gpointer udata);
void cursor_default(GtkEventControllerMotion *button, double x, double y,
                    gpointer udata);

#endif // APPSTATE_H
