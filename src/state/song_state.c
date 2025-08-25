#include "song_state.h"
#include "../songplayer/songplayer.h"

void song_state(GtkButton *btn, gpointer udata) {
  (void)btn;

  Song *song = (Song *)udata;

  printf("Current song state: %d\n", song->state);
  printf("Song path: %s\n", song->path);
  switch (song->state) {
  case SONG_STATE_IDLE: {

    song_play(song);
    song->state = SONG_STATE_PLAYING;
    return;
  }
  case SONG_STATE_PAUSED: {
    song_resume(song);
    song->state = SONG_STATE_PLAYING;
    return;
  }

  case SONG_STATE_PLAYING: {
    song_pause(song);
    song->state = SONG_STATE_PAUSED;
    return;
  }
  }
}