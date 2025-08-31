

#include "music_view.h"

#include "../state/appstate.h"

void music_view(GtkButton *button, gpointer udata) {
  (void)button;
  AppState *state = (AppState *)udata;
  //   gtk_widget_set_visible(state->songsGrid, TRUE);
  //   gtk_widget_set_visible(state->playlistGrid, FALSE);
  gtk_stack_set_visible_child_name(GTK_STACK(state->viewStack), "songs");
}