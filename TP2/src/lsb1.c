#include "lsb1.h"
#include <string.h>

void lsb1_embed(struct image* img, struct data* data) {

    assert(data->len * 8 > lsb1_bit_capacity(img->pixels));
    size_t byteOnImage = BMP_HEADER_SIZE;

    for (size_t byte = 0; byte < data->len; byte++) {

        for (size_t bitOnByte = 0; bitOnByte < 8; bitOnByte++, byteOnImage++) {
            img->raw[byteOnImage] ^= img->raw[byteOnImage] & 0x1;
            img->raw[byteOnImage] |= (data->bytes[byte / 8] >> bitOnByte) & 0x1;
        }

    }
}

void lsb1_extract(struct image* source, struct data* out) {

    size_t maxOutputSize = (3 * source->pixels) / 8;
    unsigned char* buffer = malloc(sizeof(unsigned char) * maxOutputSize);
    memset(buffer, 0, maxOutputSize);

    size_t bytesInImage = BMP_HEADER_SIZE + source->pixels * 3;

    size_t byte = 0;
    size_t bitOnByte = 0;

    for (size_t byteOnImage = BMP_HEADER_SIZE; byteOnImage < bytesInImage; byteOnImage++) {

        buffer[byte] |= (source->raw[byteOnImage] & 0x1) >> bitOnByte;

        bitOnByte++;
        if (bitOnByte == 8) {
            byte++;
            bitOnByte = 0;
        }

    }

    out->len = maxOutputSize;
    out->bytes = buffer;
}

size_t lsb1_bit_capacity(size_t pixelsInImage) {
    return pixelsInImage * 3;
}

