#ifndef APPSTATE_H
#define APPSTATE_H

#include "../helpers/vecbox.h"
#include "../songplayer/songplayer.h"
#include <gtk/gtk.h>
struct _AppState {
  GtkWidget *window;
  GtkWidget *sidebarGrid;
  GtkWidget *songTitle;
  GtkWidget *songArtist;

  size_t rowCount;
  VecBox *musicsCards;
  const char *selectedPath;
  Song *song;
};

typedef struct _AppState AppState;

#endif // APPSTATE_H