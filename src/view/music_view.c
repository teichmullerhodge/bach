

#include "music_view.h"

#include "../helpers/music_utils.h"
#include "../metadata_r/metadata_r.h"
#include "../state/appstate.h"
#include "../widgets/widget_music_card.h"
void music_view(GtkButton *button, gpointer udata) {
  (void)button;
  AppState *state = (AppState *)udata;
  //   gtk_widget_set_visible(state->songsGrid, TRUE);
  //   gtk_widget_set_visible(state->playlistGrid, FALSE);
  gtk_stack_set_visible_child_name(GTK_STACK(state->viewStack), "songs");
}

bool new_music_card(AppState *state, const char *path) {
  MetadataR *mref = collect_metadata(path);

  if (!mref)
    return false;

  // bool valid = true;

  // printf("aritst: %s\n", mref->artist);
  // printf("title: %s\n", mref->title);
  // printf("year: %u\n", mref->year);
  // printf("seconds: %lu\n", mref->seconds);

  // if (!is_valid_metadata_info(mref->title))
  //   valid = false;
  // if (!is_valid_metadata_info(mref->artist))
  //   valid = false;
  // if (mref->year == 0 || mref->year > 2100)
  //   valid = false;
  // if (mref->seconds <= 0 ||
  //     mref->seconds > 86400 * 10 /* 10 days of audio. */) {
  //   valid = false;
  // }

  // if (!valid) {
  //   g_print("⚠️ Ignoring invalid file: %s\n", path);
  //   return false;
  // }

  const char *title = mref->title == NULL || strlen(mref->title) == 0
                          ? basename_no_ext(mref->path)
                          : mref->title;
  const char *artist = mref->artist == NULL || strlen(mref->artist) == 0
                           ? "Unknown artist"
                           : mref->artist;

  GtkWidget *song = music_card(
      "./resources/GTK.png", title, artist, mref->seconds,
      &(WidgetPositioning){TRUE, FALSE, GTK_ALIGN_CENTER, GTK_ALIGN_START});
  gtk_grid_attach(GTK_GRID(state->songsGrid), song, 0, state->musicRowCount, 1,
                  1);
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
  return true;
}
