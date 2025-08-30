#include "song_slider_state.h"
#include "../definitions/definitions.h"
#include "../helpers/formatter.h"
#include "appstate.h"
#include "glib.h"
#include "gtk/gtk.h"
#include "gtk/gtkshortcut.h"
#include "song_state.h"
#include <stdio.h>
#include <stdlib.h>

gboolean on_slider_released(GtkEventController *controller, GdkEvent *event,
                            gpointer udata) {
  (void)controller;
  (void)event;
  (void)udata;
  GdkEventType type = gdk_event_get_event_type(event);
  if (type != GDK_BUTTON_RELEASE) {
    return FALSE; // ignore other events
  }

  AppState *state = (AppState *)udata;
  if (is_stream_valid(state) && is_song_playing(state)) {
    double value = gtk_range_get_value(GTK_RANGE(state->songSlider));
    i64 val = (i64)(value * G_USEC_PER_SEC);
    gtk_media_stream_seek(state->song->stream, val);
    printf("Button rewinded :)\n");
    return FALSE;
  }
  printf("Button released but there's no song being played.!\n");

  return FALSE;
}

void on_slider_change(GtkRange *self, gpointer udata) {
  (void)self;
  (void)udata;
  AppState *state = (AppState *)udata;
  char minBuf[10];
  double value = gtk_range_get_value(self);
  seconds_to_string((i32)value, minBuf, sizeof(minBuf));
  gtk_label_set_label(GTK_LABEL(state->minLabel), minBuf);

  printf("Value changed to: %f\n", value);
  return;
}

gboolean update_slider_cb(gpointer udata) {
  AppState *state = (AppState *)udata;
  if (is_stream_valid(state) && is_song_playing(state)) {

    i64 dur = gtk_media_stream_get_duration(state->song->stream);
    if (dur > 0) {
      char minBuf[10];
      double rangeVal = gtk_range_get_value(GTK_RANGE(state->songSlider));
      seconds_to_string((i32)rangeVal, minBuf, sizeof(minBuf));
      gtk_range_set_value(GTK_RANGE(state->songSlider), rangeVal + 0.1f);
      gtk_label_set_label(GTK_LABEL(state->minLabel), minBuf);
    }
    return TRUE;
  }
  return TRUE;
}