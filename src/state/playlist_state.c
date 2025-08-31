#include "playlist_state.h"
#include "../store/musics_store.h"
#include "../store/playlist_store.h"
#include "../store/store.h"
#include "../widgets/widget_playlist_card.h"
#include "appstate.h"
#include "gtk/gtk.h"

void add_new_playlist_card(GtkButton *button, gpointer udata) {
  (void)button;
  AppState *state = (AppState *)udata;

  GtkWidget *pc = playlist_card(
      "playlist-card",
      &(WidgetPositioning){TRUE, FALSE, GTK_ALIGN_CENTER, GTK_ALIGN_CENTER});

  GtkWidget *browseSongsBtn = gtk_widget_get_last_child(pc);
  GtkWidget *label = gtk_widget_get_first_child(pc);
  GtkWidget *playbtn = gtk_widget_get_next_sibling(GTK_WIDGET(label));

  g_signal_connect(browseSongsBtn, "clicked",
                   G_CALLBACK(on_playlist_browse_songs_clicked), state);

  PlaylistPlayerContext *playerContext = malloc(sizeof(PlaylistPlayerContext));
  if (!playerContext) {
    perror("Failed to allocate memory for PlaylistPlayerContext\n");
    return;
  }
  playerContext->state = state;
  playerContext->playlistCard = pc;

  g_signal_connect(playbtn, "clicked", G_CALLBACK(on_playlist_play_clicked),
                   playerContext);

  gtk_grid_attach(GTK_GRID(state->playlistGrid), pc, 0, state->playlistRowCount,
                  1, 1);
  state->playlistRowCount++;
}

void append_new_playlist_card(const char *label, AppState *state) {
  GtkWidget *pc = playlist_card_with_label(
      label, "playlist-card",
      &(WidgetPositioning){TRUE, FALSE, GTK_ALIGN_CENTER, GTK_ALIGN_CENTER});

  GtkWidget *browseSongsBtn = gtk_widget_get_last_child(pc);
  g_signal_connect(browseSongsBtn, "clicked",
                   G_CALLBACK(on_playlist_browse_songs_clicked), state);

  GtkWidget *labelWidget = gtk_widget_get_first_child(pc);
  GtkWidget *playbtn = gtk_widget_get_next_sibling(GTK_WIDGET(labelWidget));

  PlaylistPlayerContext *playerContext = malloc(sizeof(PlaylistPlayerContext));
  if (!playerContext) {
    perror("Failed to allocate memory for PlaylistPlayerContext\n");
    return;
  }
  playerContext->state = state;
  playerContext->playlistCard = pc;

  g_signal_connect(playbtn, "clicked", G_CALLBACK(on_playlist_play_clicked),
                   playerContext);

  gtk_grid_attach(GTK_GRID(state->playlistGrid), pc, 0, state->playlistRowCount,
                  1, 1);
  state->playlistRowCount++;
}

void on_playlist_play_clicked(GtkButton *button, gpointer udata) {
  (void)button;
  char fullPlaylistPath[1024];
  PlaylistPlayerContext *ctx = (PlaylistPlayerContext *)udata;

  GtkWidget *label = gtk_widget_get_first_child(ctx->playlistCard);
  const char *playlistName = gtk_editable_get_text(GTK_EDITABLE(label));

  snprintf(fullPlaylistPath, sizeof(fullPlaylistPath), PLAYLIST_STORE_DIR_FILE,
           getenv("HOME"), playlistName);

  load_musics_from_path(ctx->state, fullPlaylistPath);
  ctx->state->lastSelectedMusicRow = 0;

  return;
}

void on_playlist_files_selected(GObject *source, GAsyncResult *res,
                                gpointer udata) {

  PlaylistContext *ctx = udata;
  GtkFileDialog *dialog = GTK_FILE_DIALOG(source);

  // Multiple files
  GListModel *files = gtk_file_dialog_open_multiple_finish(dialog, res, NULL);
  if (!files)
    return;
  // free the previous store
  if (ctx->state->playlistStore)
    kv_free(ctx->state->playlistStore);
  ctx->state->playlistStore = kv_new();

  guint n = g_list_model_get_n_items(files);
  for (guint i = 0; i < n; i++) {
    GFile *file = g_list_model_get_item(files, i);
    char *path = g_file_get_path(file);

    if (path) {
      g_print("File %u: %s\n", i, path);
      char key[16];
      generate_random_key(key, sizeof(key));

      kv_set(ctx->state->playlistStore, key, path);
      g_free(path);
    }

    g_object_unref(file);
  }
  char fullPath[1024];
  snprintf(fullPath, sizeof(fullPath), PLAYLIST_STORE_DIR_FILE, getenv("HOME"),
           ctx->playlistName);
  save_kvstore(fullPath, ctx->state->playlistStore, "w");
  load_musics_from_path(ctx->state, fullPath);
  ctx->state->lastSelectedMusicRow = 0;

  g_object_unref(files);
}

void on_playlist_browse_songs_clicked(GtkButton *button, gpointer udata) {
  (void)button;
  AppState *state = (AppState *)udata;
  GtkWidget *playbtn =
      gtk_widget_get_prev_sibling(GTK_WIDGET(button)); // play - pause button
  GtkWidget *label =
      gtk_widget_get_prev_sibling(GTK_WIDGET(playbtn)); // the editable label :)

  GtkWindow *parent = GTK_WINDOW(state->window);

  PlaylistContext *ctx = malloc(sizeof(PlaylistContext));
  if (!ctx)
    return;

  ctx->state = state;
  ctx->playlistName = gtk_editable_get_text(GTK_EDITABLE(label));
  GtkFileDialog *dialog = gtk_file_dialog_new();
  gtk_file_dialog_open_multiple(dialog, parent, NULL,
                                on_playlist_files_selected, ctx);
}
