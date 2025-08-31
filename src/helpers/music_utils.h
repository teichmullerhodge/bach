#ifndef MUSIC_UTILS_H
#define MUSIC_UTILS_H

#include <stdbool.h>

bool is_same_music(const char *pA, const char *pB);
bool is_valid_metadata_info(const char *s);
char *basename_no_ext(const char *path);

#endif // MUSIC_UTILS_H