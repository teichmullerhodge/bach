#ifndef WIDGET_MUSIC_CARD_H
#define WIDGET_MUSIC_CARD_H

#include "widgets_config.h"

GtkWidget *music_card(const char *imagepath, const char *songTitle,
                      const char *author, WidgetPositioning *pos);

void select_song(GtkGestureClick *gesture, int npress, double x, double y,
                 gpointer udata);

#endif