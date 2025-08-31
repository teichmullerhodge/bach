#ifndef PLAYLIST_STATE_H
#define PLAYLIST_STATE_H

#include "appstate.h"
#include <gtk/gtk.h>

struct _PlaylistContext {
  const char *playlistName;
  AppState *state;
};

typedef struct _PlaylistContext PlaylistContext;

struct _PlaylistPlayerContext {
  AppState *state;
  GtkWidget *playlistCard;
};

typedef struct _PlaylistPlayerContext PlaylistPlayerContext;

void add_new_playlist_card(GtkButton *button, gpointer udata);
void append_new_playlist_card(const char *label, AppState *state);

void on_playlist_play_clicked(GtkButton *button, gpointer udata);

void on_playlist_files_selected(GObject *source, GAsyncResult *res,
                                gpointer udata);

void on_playlist_browse_songs_clicked(GtkButton *button, gpointer udata);
#endif // PLAYLIST_STATE_H