#ifndef __lsb1__
#define __lsb1__

#include "common.h"

void lsb1_embed(struct image* img, struct data* data);

void lsb1_extract(struct image* source, struct data* out);

size_t lsb1_bit_capacity(size_t pixelsInImage);

#endif
