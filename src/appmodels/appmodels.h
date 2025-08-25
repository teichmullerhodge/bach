#ifndef APPMODELS_H
#define APPMODELS_H

#include <gtk/gtk.h>

typedef struct {

  GtkWidget *card;
  const char *title;
  const char *artist;

} MusicCard;

#endif // APPMODELS_H