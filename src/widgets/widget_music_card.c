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
                      const char *author, u64 duration,
                      WidgetPositioning *pos) {

  GtkEventController *ctrl = gtk_event_controller_motion_new();

  GtkWidget *infoContainer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  GtkWidget *container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

  GtkWidget *image = widget_image(imagepath, 40, NULL, NULL);
  GtkWidget *song = widget_label(songTitle, "music-card-title", NULL);
  GtkWidget *aut = widget_label(author, "music-card-author", NULL);

  g_signal_connect(ctrl, "enter", G_CALLBACK(cursor_pointer), container);
  g_signal_connect(ctrl, "leave", G_CALLBACK(cursor_default), container);

  gtk_box_append(GTK_BOX(infoContainer), song);
  gtk_box_append(GTK_BOX(infoContainer), aut);

  gtk_box_append(GTK_BOX(container), image);
  gtk_box_append(GTK_BOX(container), infoContainer);

  g_object_set_data(G_OBJECT(container), "song-title", (char *)songTitle);
  g_object_set_data(G_OBJECT(container), "song-artist", (char *)author);

  double *seconds = g_new(double, 1);
  *seconds = duration;
  g_object_set_data_full(G_OBJECT(container), "song-seconds", seconds, g_free);

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

  printf("Select song event\n");

  GtkWidget *card =
      gtk_event_controller_get_widget(GTK_EVENT_CONTROLLER(gesture));
  AppState *state = (AppState *)udata;

  const char *path = gtk_widget_get_name(card);

  if (state->lastSelectedCard == NULL) {
    state->lastSelectedCard = card;
    gtk_widget_add_css_class(card, "music-selected");
  } else {
    gtk_widget_remove_css_class(state->lastSelectedCard, "music-selected");
    gtk_widget_add_css_class(card, "music-selected");
    state->lastSelectedCard = card;
  }

  char *songTitle = g_object_get_data(G_OBJECT(card), "song-title");
  char *artist = g_object_get_data(G_OBJECT(card), "song-artist");
  double *duration = g_object_get_data(G_OBJECT(card), "song-seconds");

  state->selectedPath = path;
  if (state->song)
    state->song->path = path;

  f32 seconds = *duration;
  state->song->artist = songTitle;
  state->song->title = artist;

  char maxText[9];
  char minLabel[] = "0:00";
  snprintf(maxText, 9, "%f", (double)seconds);
  gtk_label_set_label(GTK_LABEL(state->maxLabel), maxText);
  gtk_label_set_label(GTK_LABEL(state->minLabel), minLabel);
  gtk_range_set_range(GTK_RANGE(state->songSlider), 0.0f, (f32)seconds);

  gtk_range_set_value(GTK_RANGE(state->songSlider), 0.0f);

  printf("Min: %f - Max: %f\n", 0.0f, (f32)seconds);

  // gtk_label_set_text(GTK_LABEL(state->songTitle),
  //                    songTitle != NULL ? songTitle : "Unknown title");
  // gtk_label_set_text(GTK_LABEL(state->songArtist),
  //                    artist != NULL ? artist : "Unknown artist");

  // g_print("Selected song: %s - %s\n", songTitle, artist);
}
