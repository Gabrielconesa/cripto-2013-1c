#ifndef __lsbe__
#define __lsbe__

#include "common.h"

void lsbe_embed(struct data* carrier, struct data* data);

void lsbe_extract(struct data* source, struct data* out);

size_t lsbe_bit_capacity(struct data* carrier);

#endif
