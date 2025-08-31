#ifndef APPSTATE_H
#define APPSTATE_H

#include "../helpers/vecbox.h"
#include "../songplayer/songplayer.h"
#include "../store/store.h"
#include <gtk/gtk.h>

struct _AppState {
  GtkWidget *window;
  GtkWidget *mainArea;

  GtkWidget *songsGrid;
  GtkWidget *playlistGrid;
  GtkWidget *sideScroll;
  GtkWidget *viewStack;

  GtkWidget *songTitle;
  GtkWidget *songArtist;
  GtkWidget *songSlider;
  GtkWidget *minLabel;
  GtkWidget *maxLabel;

  // shared layout

  GtkWidget *sliderContainer;
  GtkWidget *controlsContainer;

  KVStore *configStore;
  KVStore *musicsStore;
  KVStore *playlistStore;

  const char *selectedPath;
  Song *song;
  GtkWidget *lastSelectedCard; // keeps the reference of the lastSelectedCard.
  size_t lastSelectedMusicRow;
  size_t musicRowCount;
  size_t playlistRowCount;
};

typedef struct _AppState AppState;

void clear_songs_grid(AppState *state);
void clear_playlist_grid(AppState *state);

#endif // APPSTATE_H