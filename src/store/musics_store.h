#ifndef MUSICS_STORE_H
#define MUSICS_STORE_H

#include "../state/appstate.h"

#define MUSIC_STORE_CONF_PATH "%s/.config/furelise/music.conf"

bool load_music_store(AppState *state, const char *home);
// used in playlists
bool load_musics_from_path(AppState *state, const char *path);

bool save_music_store(AppState *state, const char *home);
#endif // MUSICS_STORE_H