#include "widget_constructor.h"

GtkWidget *widget_construct(GtkWidget *(*gtk_new_function)(void),
                            const char *cssClass, WidgetPositioning *pos) {
  GtkWidget *widget = gtk_new_function();
  set_configs(widget, cssClass, pos);
  return widget;
}
