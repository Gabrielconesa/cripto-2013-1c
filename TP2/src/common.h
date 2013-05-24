#ifndef __data__
#define __data__

#include <stdlib.h>

#define BMP_HEADER_SIZE 54

struct data {
    size_t len;
    unsigned char* bytes; // Either encripted or not. if unencrypted, should include extension
};

void free_data(struct data* data);

#endif
