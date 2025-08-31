#include "song_browser_state.h"
#include "../helpers/text_encoding.h"
#include "../store/musics_store.h"
#include "../widgets/widget_dialog.h"
#include "../widgets/widget_music_card.h"
#include "appstate.h"
#include "gtk/gtk.h"
#include "song_state.h"
#include <stdio.h>

#include "../helpers/charmpl.h"
#include "../helpers/formatter.h"

void new_song_browser(GObject *source, GAsyncResult *res, gpointer udata) {

  AppState *state = (AppState *)udata;

  MetadataR *mref = (MetadataR *)udata;
  GtkFileDialog *dialog = GTK_FILE_DIALOG(source);
  GFile *file = gtk_file_dialog_open_finish(dialog, res, NULL);

  if (file) {
    char *path = g_file_get_path(file);
    // check if the file is already in the store
    size_t count = state->musicsStore->count;
    bool exists = false;
    for (size_t k = 0; k < count; k++) {
      const char *storedPath = state->musicsStore->pairs[k].value;
      if (strcmp(storedPath, path) == 0) {
        exists = true;
        break;
      }
    }
    if (exists) {
      const char *buttons[] = {"Continue", NULL};
      widget_dialog_new(GTK_WINDOW(state->window), "File already exists.",
                        buttons, NULL, NULL);

      g_free(path);
      g_object_unref(file);
      return;
    }
    mref = collect_metadata(path);
    if (mref) {

      GtkWidget *song = music_card(
          "./resources/GTK.png", mref->title, mref->artist, mref->seconds,
          &(WidgetPositioning){TRUE, FALSE, GTK_ALIGN_CENTER, GTK_ALIGN_START});
      gtk_grid_attach(GTK_GRID(state->songsGrid), song, 0, state->musicRowCount,
                      1, 1);
      state->musicRowCount++;
      gtk_widget_set_name(
          song,
          mref->path); // set the path as the name, will be used later for ref.

      GtkGesture *click = gtk_gesture_click_new();
      gtk_widget_add_controller(song, GTK_EVENT_CONTROLLER(click));

      g_signal_connect(click, "pressed", G_CALLBACK(select_song), state);

      g_print("File found: %s\n", mref->path);
      g_print("Title: %s\n", mref->title);
      g_print("Artist: %s\n", mref->artist);
      g_print("Year: %u\n", mref->year);
      g_print("Duration: %lu seconds\n", mref->seconds);

      // add to musics store
      char randomKey[16];
      generate_random_key(randomKey, sizeof(randomKey));
      kv_set(state->musicsStore, randomKey, mref->path);
      save_music_store(state, getenv("HOME"));
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

void previous_music(GtkButton *btn, gpointer udata) {
  (void)btn;

  AppState *state = (AppState *)udata;
  if (state->lastSelectedMusicRow == 0) {
    printf("No previous songs in the list.\n");
    return; // only one music card exists. Or not even one.
  }
  size_t pos = state->lastSelectedMusicRow - 1;
  GtkWidget *previousCard =
      gtk_grid_get_child_at(GTK_GRID(state->songsGrid), 0, pos);

  // check if previousCard is NULL or NotFound widget
  if (previousCard == NULL) {
    printf("No previous card found.\n");
    return;
  }
  if (g_strcmp0(gtk_widget_get_name(previousCard), "NotFound") == 0) {
    printf("Previous card is NotFound widget, skipping.\n");
    return;
  }
  change_state_by_music_card(previousCard, state);

  state->lastSelectedMusicRow--;
}

void next_music(GtkButton *btn, gpointer udata) {
  (void)btn;

  AppState *state = (AppState *)udata;
  if (state->lastSelectedMusicRow + 1 >= state->musicRowCount) {

    printf("No more songs in the list.\n");
    return; // only one music card exists. Or not even one.
  }
  size_t pos = state->lastSelectedMusicRow + 1;
  GtkWidget *nextCard =
      gtk_grid_get_child_at(GTK_GRID(state->songsGrid), 0, pos);
  change_state_by_music_card(nextCard, state);

  state->lastSelectedMusicRow++;
}