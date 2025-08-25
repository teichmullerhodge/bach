#include "widget_image.h"

GtkWidget *widget_image(const char *path, int size, const char *cssclass,
                        WidgetPositioning *pos) {

  GtkWidget *image = gtk_image_new_from_file(path);
  gtk_image_set_pixel_size(GTK_IMAGE(image), size);

  set_configs(image, cssclass, pos);
  return image;
}
