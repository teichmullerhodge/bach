#include "appstate.h"

void clear_songs_grid(AppState *state) {
  for (size_t i = 0; i < state->musicRowCount; i++) {
    GtkWidget *child = gtk_grid_get_child_at(GTK_GRID(state->songsGrid), 0, i);
    gtk_grid_remove(GTK_GRID(state->songsGrid), child);
  }
  state->musicRowCount = 0;
}

void clear_playlist_grid(AppState *state) {
  for (size_t i = 0; i < state->playlistRowCount; i++) {
    GtkWidget *child =
        gtk_grid_get_child_at(GTK_GRID(state->playlistGrid), 0, i);
    gtk_grid_remove(GTK_GRID(state->playlistGrid), child);
  }
  state->playlistRowCount = 0;
}