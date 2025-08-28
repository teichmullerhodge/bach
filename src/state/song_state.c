#include "song_state.h"
#include "../songplayer/songplayer.h"
#include "appstate.h"

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
    return;
  }
  case SONG_STATE_PAUSED: {
    song_resume(song);
    song->state = SONG_STATE_PLAYING;
    return;
  }

  case SONG_STATE_PLAYING: {

    if (song->playingPath != NULL &&
        strcmp(song->path, song->playingPath) != 0) {
      // new song select while another one is playing.
      song_stop(song);
      song_play(song);
      song->playingPath = song->path;
      song->state = SONG_STATE_PLAYING;
      return;
    }
    song_pause(song);
    song->state = SONG_STATE_PAUSED;
    return;
  }
  }
}