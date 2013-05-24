#include "lsb1.h"
#include <string.h>

enum embed_result lsb1_embed(struct image* img, struct data* data) {

    if (data->len * 8 > img->pixels * 3) {
        return EMBED_TOO_SMALL;
    }


    size_t byteOnImage = BMP_HEADER_SIZE;

    for (size_t byte = 0; byte < data->len; byte++) {

        for (size_t bitOnByte = 0; bitOnByte < 8; bitOnByte++, byteOnImage++) {
            img->raw[byteOnImage] ^= img->raw[byteOnImage] & 0x1;
            img->raw[byteOnImage] |= (data->bytes[byte / 8] >> bitOnByte) & 0x1;
        }

    }

    return EMBED_SUCCESS;
}

int lsb1_extract(struct image* source, struct data* out) {

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

    out->len = (buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | buffer[3];
    out->bytes = malloc(sizeof(unsigned char) * out->len);
    if (out->bytes == NULL) {
        return 0;
    }

    memcpy(out->bytes, buffer + 4, out->len);

    return 0;
}

