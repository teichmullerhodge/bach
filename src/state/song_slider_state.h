#ifndef SONG_SLIDER_STATE_H
#define SONG_SLIDER_STATE_H

#include "../definitions/definitions.h"
#include "../songplayer/songplayer.h"
#include <gtk/gtk.h>

gboolean on_slider_released(GtkEventController *controller, GdkEvent *event,
                            gpointer udata);
void on_slider_change(GtkRange *self, gpointer udata);

gboolean update_slider_cb(gpointer udata);

#endif // SONG_SLIDER_STATE_H