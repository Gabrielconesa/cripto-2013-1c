#ifndef __lsb__
#define __lsb__

#include "common.h"

void lsb_embed(struct data* carrier, struct data* data, size_t n);

void lsb_extract(struct data* source, struct data* out, size_t n);

size_t lsb_bit_capacity(size_t size, size_t n);

#endif
