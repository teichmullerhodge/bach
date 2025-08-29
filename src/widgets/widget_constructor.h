#ifndef WIDGET_CONSTRUCTOR_H
#define WIDGET_CONSTRUCTOR_H

#include "widgets_config.h"

GtkWidget *widget_construct(GtkWidget *(*gtk_new_function)(void),
                            const char *cssClass, WidgetPositioning *pos);

#endif // WIDGET_CONSTRUCTOR_H