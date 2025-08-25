#include "song_browser_state.h"
#include "../widgets/widget_music_card.h"
#include "appstate.h"

void new_song_browser(GObject *source, GAsyncResult *res, gpointer udata) {

  AppState *state = (AppState *)udata;

  MetadataR *mref = (MetadataR *)udata;
  GtkFileDialog *dialog = GTK_FILE_DIALOG(source);
  GFile *file = gtk_file_dialog_open_finish(dialog, res, NULL);

  if (file) {
    char *path = g_file_get_path(file);
    mref = collect_metadata(path);
    if (mref) {

      box_push(state->musicsCards, mref);
      GtkWidget *song = music_card(
          "./resources/GTK.png", mref->title, mref->artist,
          &(WidgetPositioning){TRUE, TRUE, GTK_ALIGN_CENTER, GTK_ALIGN_CENTER});
      gtk_widget_set_size_request(song, 245, -1);
      gtk_grid_attach(GTK_GRID(state->sidebarGrid), song, 0, state->rowCount, 1,
                      1);

      g_print("File found: %s\n", mref->path);
      g_print("Title: %s\n", mref->title);
      g_print("Artist: %s\n", mref->artist);
      g_print("Year: %u\n", mref->year);
      g_print("Duration: %lu seconds\n", mref->seconds);
    }
    g_free(path);
    g_object_unref(file);
  }
}
void on_new_song_clicked(GtkButton *button, gpointer udata) {
  (void)button;
  AppState *state = (AppState *)udata;

  GtkWindow *parent = GTK_WINDOW(state->window);

  GtkFileDialog *dialog = gtk_file_dialog_new();
  gtk_file_dialog_open(dialog, parent, NULL, new_song_browser, state);
}
