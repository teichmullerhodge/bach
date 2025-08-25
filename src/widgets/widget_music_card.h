#ifndef WIDGET_MUSIC_CARD_H
#define WIDGET_MUSIC_CARD_H

#include "widgets_config.h"

GtkWidget *music_card(const char *imagepath, const char *songTitle,
                      const char *author, WidgetPositioning *pos);

#endif