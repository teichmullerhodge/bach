#ifndef SONGPLAYER_H
#define SONGPLAYER_H

#include "../definitions/definitions.h"
#include <gtk/gtk.h>

enum _SongState { SONG_STATE_IDLE, SONG_STATE_PAUSED, SONG_STATE_PLAYING };

typedef enum _SongState SongState;

struct _Song {
  const char *path;
  const char *playingPath;
  GtkMediaStream *stream;
  GtkWidget *playButton;
  const char *title;
  const char *artist;
  u64 seconds;
  u64 year;

  SongState state;
};

typedef struct _Song Song;

void song_play(Song *song);
void song_pause(Song *song);
void song_resume(Song *song);
void song_stop(Song *song);

#endif // SONGPLAYER_H