#ifndef WIDGET_SONG_SLIDER_H
#define WIDGET_SONG_SLIDER_H

#include "../definitions/definitions.h"
#include "widgets_config.h"

#define STEP_MUSIC_DEFAULT 1.0f

GtkWidget *widget_song_slider(f32 min, f32 max, f32 step, const char *cssClass,
                              WidgetPositioning *positioning);

#endif // WIDGET_SONG_SLIDER_H
