#ifndef SONG_BROWSER_STATE_H
#define SONG_BROWSER_STATE_H

#include "../metadata_r/metadata_r.h"
#include <gtk/gtk.h>

struct _SongBrowserContext {
  GtkWidget *window;
  MetadataR *metadata;
};

typedef struct _SongBrowserContext SongBrowserContext;

void new_song_browser(GObject *source, GAsyncResult *res, gpointer udata);
void on_new_song_clicked(GtkButton *button, gpointer udata);

#endif // SONG_BROWSER_STATE_H