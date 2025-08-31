#include "widget_music_card.h"

#include "../helpers/charmpl.h"
#include "../helpers/formatter.h"
#include "../state/appstate.h"
#include "../state/song_state.h"
#include "../state/state.h"
#include "glib-object.h"
#include "glib.h"
#include "gtk/gtk.h"
#include "widget_image.h"
#include "widget_label.h"
#include "widget_touchable.h"

GtkWidget *music_card(const char *imagepath, const char *songTitle,
                      const char *author, u64 duration,
                      WidgetPositioning *pos) {
  GtkEventController *ctrl = gtk_event_controller_motion_new();

  GtkWidget *infoContainer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  GtkWidget *container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

  GtkWidget *image = widget_image(imagepath, 40, "music-card-image", NULL);

  GtkWidget *song = widget_label(
      songTitle, "music-card-title",
      &(WidgetPositioning){TRUE, FALSE, GTK_ALIGN_START, GTK_ALIGN_START});

  gtk_label_set_ellipsize(GTK_LABEL(song),
                          PANGO_ELLIPSIZE_END); // corta com "..."

  GtkWidget *aut = widget_label(
      author, "music-card-author",

      &(WidgetPositioning){TRUE, FALSE, GTK_ALIGN_START, GTK_ALIGN_START}

  );
  gtk_label_set_ellipsize(GTK_LABEL(aut), PANGO_ELLIPSIZE_END);

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

  gtk_widget_set_size_request(container, 30, 30);

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

  gtk_label_set_text(GTK_LABEL(state->songTitle),
                     songTitle != NULL ? songTitle : "Unknown title");
  gtk_label_set_text(GTK_LABEL(state->songArtist),
                     artist != NULL ? artist : "Unknown artist");

  gtk_widget_set_visible(state->sliderContainer, TRUE);
  gtk_widget_set_visible(state->controlsContainer, TRUE);

  char maxText[9];
  char minLabel[] = "0:00";

  seconds_to_string(seconds, maxText, sizeof(maxText));

  gtk_label_set_label(GTK_LABEL(state->maxLabel), maxText);
  gtk_label_set_label(GTK_LABEL(state->minLabel), minLabel);
  gtk_range_set_range(GTK_RANGE(state->songSlider), 0.0f, (f32)seconds);

  gtk_range_set_value(GTK_RANGE(state->songSlider), 0.0f);

  printf("Min: %f - Max: %f\n", 0.0f, (f32)seconds);

  // start playing the song.

  song_state(GTK_BUTTON(state->song->playButton), state);

  // gtk_label_set_text(GTK_LABEL(state->songTitle),
  //                    songTitle != NULL ? songTitle : "Unknown title");
  // gtk_label_set_text(GTK_LABEL(state->songArtist),
  //                    artist != NULL ? artist : "Unknown artist");

  // g_print("Selected song: %s - %s\n", songTitle, artist);
}
