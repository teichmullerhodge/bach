#ifndef WIDGET_VOLUME_CONTROLLER_H
#define WIDGET_VOLUME_CONTROLLER_H

#include "../definitions/definitions.h"
#include "widgets_config.h"

GtkWidget *volume_controller(GtkOrientation orientation, const char *cssClass,
                             WidgetPositioning *positioning);

#endif // WIDGET_VOLUME_CONTROLLER_H