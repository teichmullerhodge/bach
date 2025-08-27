
#include "text_encoding.h"

char *sanitize_to_utf8(const char *input) {
  if (!input)
    return g_strdup("Unknown");

  if (g_utf8_validate(input, -1, NULL)) {
    return g_strdup(input);
  }

  GError *err = NULL;
  char *converted = g_convert_with_fallback(input, -1, "UTF-8", "ISO-8859-1",
                                            "?", NULL, NULL, &err);

  if (!converted) {
    g_warning("Conversion error: %s", err->message);
    g_clear_error(&err);
    return g_strdup("Invalid text");
  }

  return converted;
}