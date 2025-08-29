#include "formatter.h"
#include <stdio.h>

void seconds_to_string(int seconds, char *buf, size_t size) {
  int minutes = seconds / 60;
  int secs = seconds % 60;
  snprintf(buf, size, "%d:%02d", minutes, secs);
}
