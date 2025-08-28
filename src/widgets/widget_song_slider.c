#include "widget_song_slider.h"
#include "gtk/gtk.h"
#include "widget_label.h"
#include <stdlib.h>

GtkWidget *widget_song_slider(f32 min, f32 max, f32 step, const char *cssClass,
                              WidgetPositioning *positioning) {

  GtkWidget *containerBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
  GtkWidget *labelsRow = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
  gtk_widget_set_hexpand(labelsRow, TRUE);
  gtk_widget_set_vexpand(labelsRow, TRUE);

  GtkWidget *songSlider =
      gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, min, max, step);

  char minText[9];
  char maxText[9];

  snprintf(minText, 9, "%f", min);
  snprintf(maxText, 9, "%f", max);

  GtkWidget *minLabel = widget_label(
      minText, "slider-label",
      &(WidgetPositioning){TRUE, FALSE, GTK_ALIGN_CENTER, GTK_ALIGN_CENTER});
  GtkWidget *maxLabel = widget_label(
      maxText, "slider-label",
      &(WidgetPositioning){TRUE, FALSE, GTK_ALIGN_CENTER, GTK_ALIGN_CENTER});

  gtk_box_append(GTK_BOX(containerBox), songSlider);
  gtk_box_append(GTK_BOX(labelsRow), minLabel);
  gtk_box_append(GTK_BOX(labelsRow), maxLabel);
  gtk_box_append(GTK_BOX(containerBox), labelsRow);

  set_configs(containerBox, cssClass, positioning);

  return containerBox;
}
