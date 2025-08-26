#include "charmpl.h"

char *truncate_string(const char *input, size_t index, const char *suffix) {
  if (!input || !suffix)
    return NULL;

  size_t input_len = strlen(input);
  size_t suffix_len = strlen(suffix);

  if (input_len <= index) {
    char *copy = malloc(input_len + 1);
    if (!copy)
      return NULL;
    strcpy(copy, input);
    return copy;
  }

  size_t new_len = index + suffix_len;
  char *result = malloc(new_len + 1);
  if (!result)
    return NULL;

  strncpy(result, input, index);
  result[index] = '\0';

  strcat(result, suffix);

  return result;
}
