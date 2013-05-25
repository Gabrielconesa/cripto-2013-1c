#include "lsb.h"
#include <string.h>

void lsb_embed(struct data* img, struct data* data, size_t n) {

    assert(data->len * 8 < lsb_bit_capacity(img->len, n));
    size_t byteOnImage = BMP_HEADER_SIZE;

    unsigned char mask = (1 << n) - 1;
    for (size_t byte = 0; byte < data->len; byte++) {

        for (size_t bitOnByte = 0; bitOnByte < 8; bitOnByte += n, byteOnImage++) {
            img->bytes[byteOnImage] ^= img->bytes[byteOnImage] & mask;
            img->bytes[byteOnImage] |= (data->bytes[byte] >> bitOnByte) & mask;
        }

    }
}

void lsb_extract(struct data* source, struct data* out, size_t n) {

    size_t maxOutputSize =  lsb_bit_capacity(source->len, n) / 8;
    unsigned char* buffer = malloc(sizeof(unsigned char) * maxOutputSize);
    memset(buffer, 0, maxOutputSize);

    size_t byte = 0;
    size_t bitOnByte = 0;

    unsigned char mask = (1 << n) - 1;
    for (size_t byteOnImage = BMP_HEADER_SIZE; byteOnImage < source->len; byteOnImage++) {

        buffer[byte] |= (source->bytes[byteOnImage] & mask) << bitOnByte;

        bitOnByte += n;
        if (bitOnByte == 8) {
            byte++;
            bitOnByte = 0;
        }

    }

    out->len = maxOutputSize;
    out->bytes = buffer;
}

size_t lsb_bit_capacity(size_t size, size_t n) {
    return (size - BMP_HEADER_SIZE) * n;
}

