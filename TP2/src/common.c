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

    size_t newLength = 4 + data->len + 1;
    if (suffix) {
        newLength += strlen(suffix);
    }

    unsigned char* buffer = malloc(sizeof(unsigned char) * newLength);
    assert(buffer);

    buffer[0] = (data->len >> 24) & 0xFF;
    buffer[1] = (data->len >> 16) & 0xFF;
    buffer[2] = (data->len >> 8) & 0xFF;
    buffer[3] = data->len & 0xFF;
    memcpy(buffer + 4, data->bytes, data->len);

    if (suffix) {
        strcpy(buffer + 4 + data->len, suffix);
    }

    buffer[newLength - 1] = 0;

    free(data->bytes);

    data->bytes = buffer;
    data->len = newLength;
}

struct data* unpack_data(struct data* packed) {

    assert(packed);

    packed->len = (packed->bytes[0] << 24) + (packed->bytes[1] << 16) + (packed->bytes[2] << 8) + packed->bytes[3];

    unsigned char* buffer = malloc(packed->len);
    assert(buffer);

    memcpy(buffer, packed->bytes + 4, packed->len);
    free(packed->bytes);

    packed->bytes = buffer;

    return packed;
}

