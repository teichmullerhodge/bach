#include "music_utils.h"

#include "../metadata_r/metadata_r.h"

#include <ctype.h> // para isprint
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool is_same_music(const char *pA, const char *pB) {

  return strcmp(pA, pB) == 0;
}

bool is_valid_metadata_info(const char *s) {
  if (!s || strlen(s) == 0)
    return false;

  for (size_t i = 0; i < strlen(s); i++) {
    unsigned char c = (unsigned char)s[i];

    if (isprint(c) || isspace(c))
      continue;

    if (c >= 0x80)
      continue;

    return false;
  }
  return true;
}
