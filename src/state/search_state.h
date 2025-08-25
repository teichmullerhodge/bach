#ifndef SEARCH_STATE_H
#define SEARCH_STATE_H

#include <gtk/gtk.h>

void on_search_changed(GtkSearchEntry *entry, gpointer udata);
bool label_in_search(GtkWidget *label, const char *text);
void filter_music_cards(const char *text, GtkWidget *sidebar);

#endif // SEARCH_STATE_H
