#include "widget_playlist_card.h"
#include "../state/playlist_state.h"
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

  // g_signal_connect(playPause, "clicked",
  // G_CALLBACK(on_playlist_play_clicked),
  //                  editableLabel);

  gtk_box_append(GTK_BOX(container), editableLabel);

  gtk_box_append(GTK_BOX(container), playPause);
  gtk_box_append(GTK_BOX(container), browseSongs);

  set_configs(container, cssclass, pos);

  return container;
}

GtkWidget *playlist_card_with_label(const char *label, const char *cssclass,
                                    WidgetPositioning *pos) {

  GtkWidget *container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
  GtkWidget *editableLabel = gtk_editable_label_new(label);
  gtk_widget_add_css_class(editableLabel, "playlist-editable-label");
  GtkWidget *browseSongs =
      touchable(NULL, "audio-card-symbolic", "playlist-card-button", NULL);
  GtkWidget *playPause = touchable(NULL, "media-playback-start-symbolic",
                                   "playlist-card-button", NULL);

  // g_signal_connect(playPause, "clicked",
  // G_CALLBACK(on_playlist_play_clicked),
  //                  editableLabel);

  gtk_box_append(GTK_BOX(container), editableLabel);

  gtk_box_append(GTK_BOX(container), playPause);
  gtk_box_append(GTK_BOX(container), browseSongs);

  set_configs(container, cssclass, pos);

  return container;
}