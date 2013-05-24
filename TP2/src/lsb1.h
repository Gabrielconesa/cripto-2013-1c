#ifndef __lsb1__
#define __lsb1__

#include "common.h"

void lsb1_embed(struct data* carrier, struct data* data);

void lsb1_extract(struct data* source, struct data* out);

size_t lsb1_bit_capacity(size_t size);

#endif
