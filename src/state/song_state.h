#ifndef SONG_STATE_H
#define SONG_STATE_H

#include "appstate.h"
#include <gtk/gtk.h>

bool switch_song(Song *state);
void song_state(GtkButton *btn, gpointer udata);
bool is_song_playing(AppState *state);
bool is_stream_valid(AppState *state);
#endif // SONG_STATE_H
