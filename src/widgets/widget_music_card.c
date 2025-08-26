#include "widget_music_card.h"
#include "../helpers/charmpl.h"
#include "../state/appstate.h"
#include "../state/state.h"
#include "glib-object.h"
#include "glib.h"
#include "gtk/gtk.h"
#include "widget_image.h"
#include "widget_label.h"

GtkWidget *music_card(const char *imagepath, const char *songTitle,
                      const char *author, WidgetPositioning *pos) {

  GtkEventController *ctrl = gtk_event_controller_motion_new();

  GtkWidget *infoContainer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  GtkWidget *container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

  char *title =
      songTitle ? truncate_string(songTitle, 15, "...") : "Unknown title";
  char *artist = author ? truncate_string(author, 15, "...") : "Unknown artist";

  GtkWidget *image = widget_image(imagepath, 40, NULL, NULL);
  GtkWidget *song = widget_label(title, "music-card-title", NULL);
  GtkWidget *aut = widget_label(artist, "music-card-author", NULL);

  g_signal_connect(ctrl, "enter", G_CALLBACK(cursor_pointer), container);
  g_signal_connect(ctrl, "leave", G_CALLBACK(cursor_default), container);

  gtk_box_append(GTK_BOX(infoContainer), song);
  gtk_box_append(GTK_BOX(infoContainer), aut);

  gtk_box_append(GTK_BOX(container), image);
  gtk_box_append(GTK_BOX(container), infoContainer);

  // g_object_set_data(G_OBJECT(container), "song-title", song);
  // g_object_set_data(G_OBJECT(container), "song-artist", aut);

  if (title)
    g_object_set_data(G_OBJECT(container), "song-title", (char *)songTitle);
  if (artist)
    g_object_set_data(G_OBJECT(container), "song-artist", (char *)author);

  // g_object_set_data(G_OBJECT(container), "song-title", (char*)songTitle);
  // g_object_set_data(G_OBJECT(container), "song-artist", (char*)author);

  gtk_widget_add_controller(container, ctrl);

  set_configs(container, "music-card", pos);

  return container;
}

void select_song(GtkGestureClick *gesture, int npress, double x, double y,
                 gpointer udata) {
  (void)gesture;
  (void)npress;
  (void)x;
  (void)y;

  GtkWidget *song =
      gtk_event_controller_get_widget(GTK_EVENT_CONTROLLER(gesture));

  GtkWidget *card =
      gtk_event_controller_get_widget(GTK_EVENT_CONTROLLER(gesture));
  AppState *state = (AppState *)udata;

  const char *path = gtk_widget_get_name(song);

  char *songTitle = g_object_get_data(G_OBJECT(card), "song-title");
  char *artist = g_object_get_data(G_OBJECT(card), "song-artist");

  state->selectedPath = path;
  if (state->song)
    state->song->path = path;

  gtk_label_set_text(GTK_LABEL(state->songTitle),
                     songTitle != NULL ? songTitle : "Unknown title");
  gtk_label_set_text(GTK_LABEL(state->songArtist),
                     artist != NULL ? artist : "Unknown artist");

  g_print("Selected song: %s - %s\n", songTitle, artist);
}
