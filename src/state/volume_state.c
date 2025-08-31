#include "volume_state.h"
#include "appstate.h"
#include "gtk/gtk.h"

void on_volume_change(GtkRange *self, gpointer udata) {

  GtkWidget *volumeControl = gtk_widget_get_parent(GTK_WIDGET(self));
  GtkWidget *volumeIcon = gtk_widget_get_first_child(volumeControl);

  double val = gtk_range_get_value(self);
  printf("Volume: %f\n", val);

  AppState *state = (AppState *)udata;
  if (state->song && state->song->stream) {

    if (val >= 0.75f) {
      // change icon to high
      gtk_image_set_from_icon_name(GTK_IMAGE(volumeIcon),
                                   "audio-volume-high-symbolic");
    }

    if (val < 0.75f && val >= 0.35f) {
      // change icon to medium
      gtk_image_set_from_icon_name(GTK_IMAGE(volumeIcon),
                                   "audio-volume-medium-symbolic");
    }
    if (val < 0.35f && val > 0.0f) {
      // change icon to low
      gtk_image_set_from_icon_name(GTK_IMAGE(volumeIcon),
                                   "audio-volume-low-symbolic");
    }
    if (val == 0.0f) {
      // change icon to muted
      gtk_image_set_from_icon_name(GTK_IMAGE(volumeIcon),
                                   "audio-volume-muted-symbolic");
    }

    gtk_media_stream_set_volume(state->song->stream, val);
  }
}