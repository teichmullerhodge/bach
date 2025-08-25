#ifndef WIDGET_IMAGE_H
#define WIDGET_IMAGE_H

#include "widgets_config.h"

GtkWidget *widget_image(const char *path, int size, const char *cssclass,
                        WidgetPositioning *pos);

#endif