#ifndef WIDGETS_CONFIG_H
#define WIDGETS_CONFIG_H

#include <gtk/gtk.h>

struct _WidgetPositioning {
  gboolean hexpand;
  gboolean vexpand;
  GtkAlign halign;
  GtkAlign valign;
};

typedef struct _WidgetPositioning WidgetPositioning;

void set_widget_positioning(GtkWidget *widget, WidgetPositioning *pos);
void set_configs(GtkWidget *widget, const char *cssclass,
                 WidgetPositioning *pos);
#endif // WIDGETS_CONFIG_H