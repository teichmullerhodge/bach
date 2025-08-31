#include "playlist_store.h"
#include "../helpers/dirreader.h"
#include "../state/playlist_state.h"
#include <stddef.h>
bool load_playlist_store(AppState *state, const char *home) {
  if (state == NULL || home == NULL)
    return false;
  char playlistPath[1024];
  snprintf(playlistPath, sizeof(playlistPath), PLAYLIST_STORE_DIR, home);
  // get all the files in the directory.
  size_t filec = 0;
  printf("Loading playlists from: %s\n", playlistPath);
  char **files = get_files_in_directory(playlistPath, NULL, &filec);

  for (size_t k = 0; k < filec; k++) {
    const char *filepath = files[k];

    const char *filename = strrchr(filepath, '/');
    filename = (filename != NULL) ? filename + 1 : filepath;

    char name[256];
    snprintf(name, sizeof(name), "%s", filename);

    char *dot = strrchr(name, '.');
    if (dot != NULL) {
      *dot = '\0';
    }

    printf("Found playlist file: %s\n", name);
    append_new_playlist_card(name, state);
  }

  return true;
}