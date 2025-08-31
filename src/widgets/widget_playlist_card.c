#include "widget_playlist_card.h"
#include "gtk/gtk.h"
#include "widget_touchable.h"
#include "widgets_config.h"

GtkWidget *playlist_card(const char *cssclass, WidgetPositioning *pos) {

  GtkWidget *container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
  GtkWidget *editableLabel = gtk_editable_label_new("Playlist name...");
  gtk_widget_add_css_class(editableLabel, "playlist-editable-label");
  GtkWidget *browseSongs =
      touchable(NULL, "audio-card-symbolic", "playlist-card-button", NULL);
  GtkWidget *playPause = touchable(NULL, "media-playback-start-symbolic",
                                   "playlist-card-button", NULL);
  gtk_box_append(GTK_BOX(container), editableLabel);
  gtk_box_append(GTK_BOX(container), browseSongs);

  gtk_box_append(GTK_BOX(container), playPause);

  set_configs(container, cssclass, pos);

  return container;
}