#include "song_state.h"
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