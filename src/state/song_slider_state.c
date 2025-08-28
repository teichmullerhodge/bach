#include "song_slider_state.h"
#include "../definitions/definitions.h"
#include "appstate.h"
#include "gtk/gtk.h"
#include <stdio.h>
#include <stdlib.h>

void on_slider_change(GtkRange *range, gpointer *udate) {
  (void)range;

  AppState *appState = (AppState *)udate;

  if (appState->song != NULL) {

    f64 where = gtk_range_get_value(range);
    char minText[9];
    snprintf(minText, 9, "%f", where);
    gtk_label_set_label(GTK_LABEL(appState->minLabel), minText);
    printf("Slider changed to: %f\n", where);
  }

  if (appState->song->stream == NULL) {
    printf("Nothing...\n");
    return;
  }

  f64 where = gtk_range_get_value(range);
  char minText[9];
  snprintf(minText, 9, "%f", where);
  gtk_label_set_label(GTK_LABEL(appState->minLabel), minText);
  printf("Slider changed to: %f\n", where);

  gtk_media_stream_seek(appState->song->stream, where * G_TIME_SPAN_SECOND);
  printf("Changed the media location\n");
}