#include "song_slider_state.h"
#include "../definitions/definitions.h"
#include "appstate.h"

void on_slider_change(GtkRange *range, gpointer *udate) {
  (void)range;

  AppState *appState = (AppState *)udate;

  if (appState->song == NULL || appState->song->stream == NULL) {
    printf("Nothing...\n");
    return;
  }

  f64 where = gtk_range_get_value(range);
  printf("Slider changed to: %f\n", where);
  gtk_media_stream_seek(appState->song->stream, where * G_TIME_SPAN_SECOND);
  printf("Changed the media location\n");
}