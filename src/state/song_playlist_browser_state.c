#include "song_playlist_browser_state.h"

void on_playlist_files_selected(GObject *source, GAsyncResult *res,
                                gpointer udata) {

  AppState *state = udata;
  (void)state;
  GtkFileDialog *dialog = GTK_FILE_DIALOG(source);

  // Multiple files
  GListModel *files = gtk_file_dialog_open_multiple_finish(dialog, res, NULL);
  if (!files)
    return;

  guint n = g_list_model_get_n_items(files);
  for (guint i = 0; i < n; i++) {
    GFile *file = g_list_model_get_item(files, i);
    char *path = g_file_get_path(file);

    if (path) {
      g_print("File %u: %s\n", i, path);

      g_free(path);
    }

    g_object_unref(file);
  }

  g_object_unref(files);
}

void on_new_playlist_clicked(GtkButton *button, gpointer udata) {
  (void)button;
  AppState *state = (AppState *)udata;

  GtkWindow *parent = GTK_WINDOW(state->window);

  GtkFileDialog *dialog = gtk_file_dialog_new();
  gtk_file_dialog_open_multiple(dialog, parent, NULL,
                                on_playlist_files_selected, state);
}
