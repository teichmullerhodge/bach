#ifndef DIRREADER_H
#define DIRREADER_H

#include "../state/appstate.h"

/// load from the standard music directory
bool load_music_cards(AppState *appState);

char **get_files_in_directory(const char *directory,
                              const char **excludesPrefix, size_t *filec);

#endif // DIRREADER_H