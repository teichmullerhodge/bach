#include "musics_store.h"
#include "../view/music_view.h"
#include "gtk/gtk.h"
#include "store.h"

bool load_music_store(AppState *state, const char *home) {

  char musicsPath[1024];
  snprintf(musicsPath, sizeof(musicsPath), MUSIC_STORE_CONF_PATH, home);
  bool storeLoaded = load_kvstore(musicsPath, state->musicsStore);
  if (!storeLoaded) {
    perror("Music store load failed.\n");
    return false;
  }
  size_t count = state->musicsStore->count;
  for (size_t k = 0; k < count; k++) {
    // music path are the value, the key is not relevant in this case.
    const char *path = state->musicsStore->pairs[k].value;
    printf("Path of the song is: %s\n", path);
    bool cardCreated = new_music_card(state, path);
    if (!cardCreated) {
      g_print("Failed to create music card for path: %s\n", path);
      continue;
    }
  }

  return true;
}

bool load_musics_from_path(AppState *state, const char *path) {
  // clear the previous infos from the grid inside state
  clear_songs_grid(state);
  KVStore *tmp = malloc(sizeof(KVStore));
  if (!tmp) {
    perror("Music store malloc failed.\n");
    return false;
  }

  bool storeLoaded = load_kvstore(path, tmp);
  if (!storeLoaded) {
    perror("Music store load failed.\n");
    return false;
  }
  printf("Path of the songs being loaded is: %s\n", path);

  size_t count = tmp->count;
  for (size_t k = 0; k < count; k++) {
    const char *path = tmp->pairs[k].value;
    bool cardCreated = new_music_card(state, path);
    if (!cardCreated) {
      g_print("Failed to create music card for path: %s\n", path);
      continue;
    }
  }
  free(tmp);
  gtk_stack_set_visible_child_name(GTK_STACK(state->viewStack), "songs");
  return true;
}

bool save_music_store(AppState *state, const char *home) {
  char musicsPath[1024];
  snprintf(musicsPath, sizeof(musicsPath), MUSIC_STORE_CONF_PATH, home);
  bool storeSaved = save_kvstore(musicsPath, state->musicsStore, "a");
  if (!storeSaved) {
    perror("Music store save failed.\n");
    return false;
  }
  return true;
}