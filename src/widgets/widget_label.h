#ifndef WIDGET_LABEL_H
#define WIDGET_LABEL_H

#include "widgets_config.h"

GtkWidget *widget_label(const char *text, const char *cssclass,
                        WidgetPositioning *pos);

#endif