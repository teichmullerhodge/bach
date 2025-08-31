#ifndef PLAYLIST_STORE_H
#define PLAYLIST_STORE_H

#include "../state/appstate.h"
#define PLAYLIST_STORE_DIR "%s/.config/furelise/playlists"
#define PLAYLIST_STORE_DIR_FILE "%s/.config/furelise/playlists/%s.playl"

bool load_playlist_store(AppState *state, const char *home);

#endif // PLAYLIST_STORE_H