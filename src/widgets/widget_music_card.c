#include "widget_music_card.h"
#include "../state/state.h"
#include "glib-object.h"
#include "glib.h"
#include "gtk/gtk.h"
#include "widget_image.h"
#include "widget_label.h"

#include "../state/appstate.h"

GtkWidget *music_card(const char *imagepath, const char *songTitle,
                      const char *author, WidgetPositioning *pos) {

  GtkEventController *ctrl = gtk_event_controller_motion_new();

  GtkWidget *infoContainer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  GtkWidget *container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

  GtkWidget *image = widget_image(imagepath, 40, NULL, NULL);
  GtkWidget *song = widget_label(songTitle, "music-card-title", NULL);
  GtkWidget *aut = widget_label(author, "music-card-author", NULL);

  gtk_box_append(GTK_BOX(infoContainer), song);
  gtk_box_append(GTK_BOX(infoContainer), aut);

  gtk_box_append(GTK_BOX(container), image);
  gtk_box_append(GTK_BOX(container), infoContainer);

  g_signal_connect(ctrl, "enter", G_CALLBACK(cursor_pointer), container);
  g_signal_connect(ctrl, "leave", G_CALLBACK(cursor_default), container);

  gtk_widget_add_controller(container, ctrl);

  set_configs(container, "music-card", pos);

  return container;
}

void select_song(GtkGestureClick *gesture, int npress, double x, double y,
                 gpointer udata) {
  (void)gesture;
  (void)npress;
  (void)x;
  (void)y;
  GtkWidget *song =
      gtk_event_controller_get_widget(GTK_EVENT_CONTROLLER(gesture));

  AppState *state = (AppState *)udata;

  const char *path = gtk_widget_get_name(GTK_WIDGET(song));
  state->song->path = path;
  state->selectedPath = path; // to be accessible easier in the app state.
  g_print("Selected song path: %s\n", state->song->path);
}