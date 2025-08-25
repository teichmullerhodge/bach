#include "metadata_r.h"
#include <stdio.h>
#include <stdlib.h>
#include <taglib/tag_c.h>

MetadataR *collect_metadata(const char *path) {
  TagLib_File *file = taglib_file_new(path);
  if (file == NULL) {
    perror("Error opening file to collect metadata. Check your path.\n");
    return NULL;
  }
  MetadataR *metadata = malloc(sizeof(MetadataR));
  if (!metadata) {
    perror("Error while allocating memory for the metadata.\n");
    return NULL;
  }

  const TagLib_Tag *tag = taglib_file_tag(file);
  if (tag) {
    metadata->title = taglib_tag_title(tag);
    metadata->artist = taglib_tag_artist(tag);
    metadata->year = taglib_tag_year(tag);
  }

  const TagLib_AudioProperties *prop = taglib_file_audioproperties(file);
  if (prop) {
    metadata->seconds = taglib_audioproperties_length(prop);
  }
  metadata->path = path;
  taglib_file_free(file);
  return metadata;
}
void destruct_metadata(MetadataR *metadata) {
  free(metadata);
  metadata = NULL;
}
