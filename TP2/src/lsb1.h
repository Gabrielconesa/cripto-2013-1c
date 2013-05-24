#ifndef __lsb1__
#define __lsb1__

#include "common.h"

enum embed_result lsb4_embed(struct image* img, struct data* data);

int lsb4_extract(struct image* source, struct data* out);

#endif
