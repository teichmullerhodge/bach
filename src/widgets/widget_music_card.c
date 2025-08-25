#include "widget_music_card.h"
#include "../state/state.h"
#include "glib-object.h"
#include "glib.h"
#include "gtk/gtk.h"
#include "widget_image.h"
#include "widget_label.h"

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
