#include "vecbox.h"
#include <stdlib.h>

VecBox *box_new(void) {
  VecBox *box = malloc(sizeof(VecBox));
  if (box == NULL)
    return NULL;
  box->data = malloc(sizeof(void *) * BOX_INITIAL_CAPACITY);
  if (box->data == NULL) {
    free(box);
    return NULL;
  }
  box->size = BOX_INITIAL_CAPACITY;
  box->_i = 0;
  box->reserved = false;
  return box;
}

bool reserve_box(VecBox *box) {
  if (box == NULL)
    return false;
  box->reserved = true;
  return true;
}

int box_push(VecBox *box, void *data) {
  if (box->reserved)
    return -1;
  if (box == NULL)
    return -1;
  if (box->_i == box->size) {
    box->size = box->size != 0 ? box->size << 1 : BOX_INITIAL_CAPACITY;
    box->data = realloc(box->data, sizeof(void *) * box->size);
    if (box->data == NULL)
      return -1;
  }

  box->data[box->_i] = data;
  box->_i++;
  return true;
}

int box_free(VecBox *box) {
  if (box == NULL)
    return -1;
  free(box->data);
  free(box);
  box = NULL;
  return true;
}