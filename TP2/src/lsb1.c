#include "lsb1.h"
#include <string.h>

void lsb1_embed(struct data* img, struct data* data) {

    assert(data->len * 8 < lsb1_bit_capacity(img->len));
    size_t byteOnImage = BMP_HEADER_SIZE;

    for (size_t byte = 0; byte < data->len; byte++) {

        for (size_t bitOnByte = 0; bitOnByte < 8; bitOnByte++, byteOnImage++) {
            img->bytes[byteOnImage] ^= img->bytes[byteOnImage] & 0x1;
            img->bytes[byteOnImage] |= (data->bytes[byte] >> bitOnByte) & 0x1;
        }

    }
}

void lsb1_extract(struct data* source, struct data* out) {

    size_t maxOutputSize =  lsb1_bit_capacity(source->len) / 8;
    unsigned char* buffer = malloc(sizeof(unsigned char) * maxOutputSize);
    memset(buffer, 0, maxOutputSize);

    size_t byte = 0;
    size_t bitOnByte = 0;

    for (size_t byteOnImage = BMP_HEADER_SIZE; byteOnImage < source->len; byteOnImage++) {

        buffer[byte] |= (source->bytes[byteOnImage] & 0x1) << bitOnByte;

        bitOnByte++;
        if (bitOnByte == 8) {
            byte++;
            bitOnByte = 0;
        }

    }

    out->len = maxOutputSize;
    out->bytes = buffer;
}

size_t lsb1_bit_capacity(size_t size) {
    return size - BMP_HEADER_SIZE;
}

