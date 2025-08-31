#ifndef STORE_H
#define STORE_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct {
  char key[128];
  char value[512];
} KVPair;

typedef struct {
  int count;
  int capacity;
  KVPair *pairs;
} KVStore;

struct _MusicsRawStore {
  size_t count;
  KVPair *pairs;
};

typedef struct _MusicsRawStore MusicsRawStore;

struct _PlaylistRawStore {
  size_t count;
  KVPair *pairs;
};

typedef struct _PlaylistRawStore PlaylistRawStore;

KVStore *kv_new(void);

void generate_random_key(char *buffer, size_t length);

void kv_init(KVStore *store);
void kv_free(KVStore *store);

bool load_kvstore(const char *filename, KVStore *store);
bool save_kvstore(const char *filename, KVStore *store, const char *mode);

const char *kv_get(KVStore *store, const char *key);
void kv_set(KVStore *store, const char *key, const char *value);

void kv_print(KVStore *store);

#endif // STORE_H
