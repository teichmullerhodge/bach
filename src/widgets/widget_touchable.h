#ifndef WIDGET_TOUCHABLE_H
#define WIDGET_TOUCHABLE

#include "widgets_config.h"

GtkWidget *touchable(const char *label, const char *icon, const char *cssClass,
                     WidgetPositioning *positioning);

#endif // WIDGET_TOUCHABLE_H
