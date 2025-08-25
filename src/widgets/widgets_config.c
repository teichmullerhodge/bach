#include "widgets_config.h"

void set_widget_positioning(GtkWidget *widget, WidgetPositioning *pos) {
  if (widget == NULL || pos == NULL)
    return;
  gtk_widget_set_hexpand(widget, pos->hexpand);
  gtk_widget_set_vexpand(widget, pos->vexpand);
  gtk_widget_set_halign(widget, pos->halign);
  gtk_widget_set_valign(widget, pos->valign);
}

void set_configs(GtkWidget *widget, const char *cssclass,
                 WidgetPositioning *pos) {
  if (widget == NULL)
    return;

  if (cssclass != NULL)
    gtk_widget_add_css_class(widget, cssclass);

  if (pos != NULL)
    set_widget_positioning(widget, pos);
}