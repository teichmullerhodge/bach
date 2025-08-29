#include "song_slider_state.h"
#include "../definitions/definitions.h"
#include "../helpers/formatter.h"
#include "appstate.h"
#include "gtk/gtk.h"
#include "gtk/gtkshortcut.h"
#include <stdio.h>
#include <stdlib.h>

void on_slider_release(GtkGestureClick *gesture, int npress, double x, double y,
                       gpointer udata) {

  (void)npress;
  (void)x;
  (void)y;
  printf("Slider release event triggered...\n");
  AppState *appState = (AppState *)udata;

  if (appState == NULL || appState->song == NULL) {
    printf("No song loaded...\n");
    return;
  }

  if (appState->song->stream == NULL) {
    printf("Stream is NULL...\n");
    return;
  }

  GtkWidget *slider =
      gtk_event_controller_get_widget(GTK_EVENT_CONTROLLER(gesture));
  if (slider == NULL) {
    printf("Slider widget is NULL...\n");
    return;
  }

  double value = gtk_range_get_value(GTK_RANGE(slider));

  char minText[9];
  seconds_to_string((int)value, minText, sizeof(minText));
  gtk_label_set_label(GTK_LABEL(appState->minLabel), minText);
  printf("Slider released at: %f\n", value);

  gtk_media_stream_seek(appState->song->stream, value * G_TIME_SPAN_SECOND);
  printf("Changed the media location\n");

  return;
}

gboolean update_song_slider(gpointer udata) {
  if (udata == NULL) {
    return G_SOURCE_CONTINUE;
  }
  AppState *state = (AppState *)udata;
  if (state->song == NULL || state->songSlider == NULL) {
    return G_SOURCE_CONTINUE;
  }
  if (state->song->state == SONG_STATE_PLAYING && state->song->stream != NULL) {
    printf("Updating song slider...\n");
    GtkWidget *songSlider = state->songSlider;
    double pos = gtk_range_get_value(GTK_RANGE(songSlider));
    gtk_range_set_value(GTK_RANGE(state->songSlider), pos + 1.0f);
  }

  return G_SOURCE_CONTINUE;
}