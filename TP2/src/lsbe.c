#include "lsbe.h"
#include <string.h>

#define CAN_EMBED(byte) (((byte) & 0xFE) == 0xFE)

void lsbe_embed(struct data* img, struct data* data) {

    assert(data->len * 8 < lsbe_bit_capacity(img));
    size_t byteOnImage = BMP_HEADER_SIZE;

    for (size_t byte = 0; byte < data->len; byte++) {

        for (size_t bitOnByte = 0; bitOnByte < 8; bitOnByte += 1, byteOnImage++) {

            while (!CAN_EMBED(img->bytes[byteOnImage])) {
                byteOnImage++;
            }

            img->bytes[byteOnImage] ^= img->bytes[byteOnImage] & 0x1;
            img->bytes[byteOnImage] |= (data->bytes[byte] >> bitOnByte) & 0x1;
        }

    }
}

void lsbe_extract(struct data* source, struct data* out) {

    size_t maxOutputSize = lsbe_bit_capacity(source) / 8;
    unsigned char* buffer = malloc(sizeof(unsigned char) * maxOutputSize);
    memset(buffer, 0, maxOutputSize);

    size_t byte = 0;
    size_t bitOnByte = 0;

    for (size_t byteOnImage = BMP_HEADER_SIZE; byte < maxOutputSize; byteOnImage++) {

        // Since maxOutputSize is computed from source, we can safely assume that
        // it will be enough to fill buffer and byteOnImage wont be biffer than the size
        while (!CAN_EMBED(source->bytes[byteOnImage])) {
            byteOnImage++;
        }

        buffer[byte] |= (source->bytes[byteOnImage] & 0x1) << bitOnByte;

        bitOnByte += 1;
        if (bitOnByte == 8) {
            byte++;
            bitOnByte = 0;
        }

    }

    out->len = maxOutputSize;
    out->bytes = buffer;
}

size_t lsbe_bit_capacity(struct data* carrier) {

    size_t count = 0;

    for (size_t byte = BMP_HEADER_SIZE; byte < carrier->len; byte++) {
        if (CAN_EMBED(carrier->bytes[byte])) {
            count++;
        }
    }

    return count;
}

