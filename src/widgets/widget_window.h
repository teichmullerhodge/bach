#ifndef WIDGET_WINDOW_H
#define WIDGET_WINDOW_H

#include "widgets_config.h"

GtkWidget *widget_window(GtkApplication *app, const char *title, i32 width,
                         i32 height);

#endif // WIDGET_WINDOW_H