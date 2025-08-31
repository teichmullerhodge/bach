#include "store.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

void generate_random_key(char *buffer, size_t length) {
  const char charset[] =
      "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  for (size_t i = 0; i < length - 1; i++) {
    int key = rand() % (int)(sizeof(charset) - 1);
    buffer[i] = charset[key];
  }
  buffer[length - 1] = '\0';
}

KVStore *kv_new(void) {
  KVStore *store = malloc(sizeof(KVStore));
  if (!store) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }
  kv_init(store);
  return store;
}

void kv_init(KVStore *store) {
  store->count = 0;
  store->capacity = 8;
  store->pairs = malloc(sizeof(KVPair) * store->capacity);
}

void kv_free(KVStore *store) {
  free(store->pairs);
  store->pairs = NULL;
  store->count = 0;
  store->capacity = 0;
}

static void kv_expand_if_needed(KVStore *store) {
  if (store->count >= store->capacity) {
    store->capacity *= 2;
    KVPair *tmp = realloc(store->pairs, sizeof(KVPair) * store->capacity);
    if (!tmp) {
      perror("realloc");
      exit(EXIT_FAILURE);
    }
    store->pairs = tmp;
  }
}

bool load_kvstore(const char *filename, KVStore *store) {
  FILE *fp = fopen(filename, "r");
  if (!fp)
    return false;

  kv_init(store);

  char line[MAX_LINE];
  while (fgets(line, sizeof(line), fp)) {
    char *eq = strchr(line, '=');
    if (!eq)
      continue;
    *eq = '\0';

    char *key = line;
    char *val = eq + 1;

    val[strcspn(val, "\r\n")] = 0;

    kv_expand_if_needed(store);

    strncpy(store->pairs[store->count].key, key,
            sizeof(store->pairs[store->count].key) - 1);
    store->pairs[store->count].key[sizeof(store->pairs[store->count].key) - 1] =
        '\0';

    strncpy(store->pairs[store->count].value, val,
            sizeof(store->pairs[store->count].value) - 1);
    store->pairs[store->count]
        .value[sizeof(store->pairs[store->count].value) - 1] = '\0';

    store->count++;
  }

  fclose(fp);
  return true;
}

bool save_kvstore(const char *filename, KVStore *store, const char *mode) {
  FILE *fp = fopen(filename, mode);
  if (!fp)
    return false;

  for (int i = 0; i < store->count; i++) {
    fprintf(fp, "%s=%s\n", store->pairs[i].key, store->pairs[i].value);
  }

  fclose(fp);
  return true;
}

const char *kv_get(KVStore *store, const char *key) {
  for (int i = 0; i < store->count; i++) {
    if (strcmp(store->pairs[i].key, key) == 0) {
      return store->pairs[i].value;
    }
  }
  return NULL;
}

void kv_set(KVStore *store, const char *key, const char *value) {
  for (int i = 0; i < store->count; i++) {
    if (strcmp(store->pairs[i].key, key) == 0) {
      strncpy(store->pairs[i].value, value, sizeof(store->pairs[i].value) - 1);
      store->pairs[i].value[sizeof(store->pairs[i].value) - 1] = '\0';
      return;
    }
  }

  kv_expand_if_needed(store);

  strncpy(store->pairs[store->count].key, key,
          sizeof(store->pairs[store->count].key) - 1);
  store->pairs[store->count].key[sizeof(store->pairs[store->count].key) - 1] =
      '\0';

  strncpy(store->pairs[store->count].value, value,
          sizeof(store->pairs[store->count].value) - 1);
  store->pairs[store->count]
      .value[sizeof(store->pairs[store->count].value) - 1] = '\0';

  store->count++;
}

void kv_print(KVStore *store) {
  for (int i = 0; i < store->count; i++) {
    printf("%s=%s\n", store->pairs[i].key, store->pairs[i].value);
  }
}