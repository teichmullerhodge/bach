#include "song_state.h"
#include "../helpers/formatter.h"
#include "../songplayer/songplayer.h"

bool switch_song(Song *song) {
  if (song->playingPath != NULL && strcmp(song->path, song->playingPath) != 0) {
    song_stop(song);
    song_play(song);
    song->playingPath = song->path;
    song->state = SONG_STATE_PLAYING;
    return true;
  }
  return false;
}

void song_state(GtkButton *btn, gpointer udata) {
  (void)btn;

  AppState *state = (AppState *)udata;
  Song *song = state->song;
  printf("Current song state: %d\n", song->state);
  printf("Song path: %s\n", song->path);
  switch (song->state) {
  case SONG_STATE_IDLE: {

    song_play(song);
    song->state = SONG_STATE_PLAYING;
    song->playingPath = song->path;
    gtk_widget_add_css_class(state->mainArea, "main-area-moving");
    return;
  }
  case SONG_STATE_PAUSED: {
    if (switch_song(song)) {
      gtk_widget_add_css_class(state->mainArea, "main-area-moving");
      return;
    }

    song_resume(song);
    song->state = SONG_STATE_PLAYING;
    gtk_widget_add_css_class(state->mainArea, "main-area-moving");
    return;
  }

  case SONG_STATE_PLAYING: {

    if (switch_song(song)) {
      gtk_widget_add_css_class(state->mainArea, "main-area-moving");
      return;
    }
    song_pause(song);
    song->state = SONG_STATE_PAUSED;
    gtk_widget_remove_css_class(state->mainArea, "main-area-moving");
    return;
  }
  }
}

bool is_song_playing(AppState *state) {
  return state->song != NULL && state->song->state == SONG_STATE_PLAYING;
}

bool is_stream_valid(AppState *state) {
  return state->song != NULL && state->song->stream != NULL;
}

void change_state_by_music_card(GtkWidget *card, AppState *state) {

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
}