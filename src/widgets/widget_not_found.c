#include "widget_not_found.h"
#include "gtk/gtk.h"

GtkWidget *not_found() {
  GtkWidget *infoContainer = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  GtkWidget *label = gtk_label_new("Nothing found");
  gtk_widget_add_css_class(label, "not-found-label");
  GtkWidget *icon = gtk_image_new_from_icon_name("face-sad-symbolic");
  gtk_widget_add_css_class(icon, "icon-not-found-label");

  gtk_box_append(GTK_BOX(infoContainer), label);
  gtk_box_append(GTK_BOX(infoContainer), icon);

  return infoContainer;
}