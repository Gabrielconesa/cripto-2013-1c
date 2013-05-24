#include <string.h>

#include "common.h"

void free_data(struct data* data) {

    assert(data);

    if (data->bytes) {
        free(data->bytes);
        data->bytes = NULL;
    }

    free(data);
}

void prepare_data(struct data* data, const char* suffix) {

    assert(data);

    size_t newLength = sizeof(size_t) + data->len + 1;
    if (suffix) {
        newLength += strlen(suffix);
    }

    unsigned char* buffer = malloc(sizeof(unsigned char) * newLength);
    assert(buffer);

    *((size_t*) buffer) = data->len;
    memcpy(buffer + sizeof(size_t), data->bytes, data->len);

    if (suffix) {
        strcpy(buffer + sizeof(size_t) + data->len, suffix);
    }

    buffer[newLength - 1] = 0;

    free(data->bytes);

    data->bytes = buffer;
    data->len = newLength;
}

