#include "widget_label.h"
#include "gtk/gtk.h"

GtkWidget *widget_label(const char *text, const char *cssclass,
                        WidgetPositioning *pos) {
  GtkWidget *label = gtk_label_new(text);
  set_configs(label, cssclass, pos);
  return label;
}
