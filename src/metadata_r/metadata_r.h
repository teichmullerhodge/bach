#ifndef METADATA_R_H
#define METADATA_R_H

#include "../definitions/definitions.h"

struct _MetadataR {
  u64 seconds;
  u32 year;
  const char *title;
  const char *artist;
  const char *path;
};

typedef struct _MetadataR MetadataR;

MetadataR *collect_metadata(const char *path);
void destruct_metadata(MetadataR *metadata);

#endif // METADATA_R_H
