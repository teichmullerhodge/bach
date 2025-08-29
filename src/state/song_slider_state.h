#ifndef SONG_SLIDER_STATE_H
#define SONG_SLIDER_STATE_H

#include "../definitions/definitions.h"
#include "../songplayer/songplayer.h"
#include <gtk/gtk.h>

void on_slider_release(GtkGestureClick *gesture, int n_press, double x,
                       double y, gpointer udata);

gboolean update_song_slider(gpointer udata);

#endif // SONG_SLIDER_STATE_H