#include "widget_volume_controller.h"
#include "gtk/gtk.h"

GtkWidget *volume_controller(GtkOrientation orientation, const char *cssClass,
                             WidgetPositioning *positioning) {

  GtkWidget *container = gtk_box_new(orientation, 5);
  GtkWidget *volumeIcon =
      gtk_image_new_from_icon_name("audio-volume-high-symbolic");
  GtkWidget *volumeSlider =
      gtk_scale_new_with_range(orientation, 0.0, 1.0, 0.01);
  gtk_widget_set_hexpand(volumeSlider, TRUE);

  gtk_range_set_value(GTK_RANGE(volumeSlider), 0.75f);

  gtk_box_append(GTK_BOX(container), volumeIcon);
  gtk_box_append(GTK_BOX(container), volumeSlider);
  set_configs(container, cssClass, positioning);
  return container;
}
