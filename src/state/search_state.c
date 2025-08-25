#include "search_state.h"
#include "../widgets/widget_not_found.h"
#include "gtk/gtk.h"
#include <string.h>

void on_search_changed(GtkSearchEntry *entry, gpointer udata) {
  (void)entry;
  const char *text = gtk_editable_get_text(GTK_EDITABLE(entry));
  filter_music_cards(text, GTK_WIDGET(udata));
}

bool label_in_search(GtkWidget *label, const char *text) {
  if (GTK_IS_LABEL(label)) {
    const char *ltext = gtk_label_get_text(GTK_LABEL(label));
    return g_strrstr(ltext, text) != NULL;
  }

  return false;
}

void filter_music_cards(const char *text, GtkWidget *sidebar) {
  (void)text;

  GtkWidget *child = gtk_widget_get_first_child(sidebar);
  GtkWidget *notFound = NULL;

  bool anyCardVisible = false;

  while (child != NULL) {
    // box are music cards
    if (strcmp(gtk_widget_get_name(child), "NotFound") == 0) {
      notFound = child;
    }

    if (GTK_IS_BOX(child)) {
      GtkWidget *infoContainer = gtk_widget_get_last_child(child);
      if (GTK_IS_BOX(infoContainer)) {

        GtkWidget *titleLabel = gtk_widget_get_first_child(infoContainer);
        GtkWidget *authorLabel = gtk_widget_get_last_child(infoContainer);

        bool showCard = label_in_search(titleLabel, text) ||
                        label_in_search(authorLabel, text);

        anyCardVisible = anyCardVisible || showCard;
        gtk_widget_set_visible(child, showCard);
      }
    }

    child = gtk_widget_get_next_sibling(child);
  }
  if (notFound != NULL) {

    gtk_widget_set_visible(notFound, !anyCardVisible);
  }
}