#include "playlist_view.h"
#include "../state/appstate.h"

void playlist_view(GtkButton *button, gpointer udata) {
  (void)button;
  AppState *state = (AppState *)udata;

  gtk_stack_set_visible_child_name(GTK_STACK(state->viewStack), "playlist");
}