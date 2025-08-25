#ifndef APPSTATE_H
#define APPSTATE_H

#include "../helpers/vecbox.h"
#include <gtk/gtk.h>
struct _AppState {
  GtkWidget *window;
  GtkWidget *sidebarGrid;
  size_t rowCount;
  VecBox *musicsCards;
};

typedef struct _AppState AppState;

#endif // APPSTATE_H