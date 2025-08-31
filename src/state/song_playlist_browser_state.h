#ifndef SONG_PLAYLIST_BROWSER_STATE_H
#define SONG_PLAYLIST_BROWSER_STATE_H

#include "appstate.h"
#include <gtk/gtk.h>

void on_playlist_files_selected(GObject *source, GAsyncResult *res,
                                gpointer udata);
void on_new_playlist_clicked(GtkButton *button, gpointer udata);

#endif // SONG_PLAYLIST_BROWSER_STATE_H