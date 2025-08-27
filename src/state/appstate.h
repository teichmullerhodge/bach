#ifndef APPSTATE_H
#define APPSTATE_H

#include "../helpers/vecbox.h"
#include "../songplayer/songplayer.h"
#include <gtk/gtk.h>
struct _AppState {
  GtkWidget *window;
  GtkWidget *sidebar;
  GtkWidget *songTitle;
  GtkWidget *songArtist;

  VecBox *musicsCards;
  const char *selectedPath;
  Song *song;
  GtkWidget *lastSelectedCard; // keeps the reference of the lastSelectedCard.
};

typedef struct _AppState AppState;

#endif // APPSTATE_H