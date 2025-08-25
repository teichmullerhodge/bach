#ifndef SONGPLAYER_H
#define SONGPLAYER_H

#include <gtk/gtk.h>

enum _SongState { SONG_STATE_IDLE, SONG_STATE_PAUSED, SONG_STATE_PLAYING };

typedef enum _SongState SongState;

struct _Song {
  const char *path;
  GtkMediaStream *stream;
  GtkWidget *playButton;
  SongState state;
};

typedef struct _Song Song;

void song_play(Song *song);
void song_pause(Song *song);
void song_resume(Song *song);

#endif // SONGPLAYER_H