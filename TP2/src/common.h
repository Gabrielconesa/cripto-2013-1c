#ifndef __data__
#define __data__

#include <stdlib.h>

#define BMP_HEADER_SIZE 54

struct data {
    size_t len;
    unsigned char* bytes; // Either encripted or not. if unencrypted, should include extension
};

struct image {
    size_t pixels;
    unsigned char* raw; // Includes header
};

enum embed_result {
    EMBED_SUCCESS,
    EMBED_TOO_SMALL
};

#endif
