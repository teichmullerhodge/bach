#ifndef VECBOX_H
#define VECBOX_H

#include <stdbool.h>
#include <stdlib.h>

#define BOX_INITIAL_CAPACITY 6

struct _VecBox {
  void **data;
  size_t size;
  size_t _i;
  bool reserved;
};

typedef struct _VecBox VecBox;

VecBox *box_new(void);
bool reserve_box(VecBox *box);
int box_push(VecBox *box, void *data);
int box_free(VecBox *box);

#endif // VECBOX_H