#include <stdio.h>

#include "lsb1.h"
#include "common.h"
#include "file.h"

int main(int argc, char *argv[]) {

    // Quick & dirty params:
    //  in file
    //  carrier
    assert(argc > 2);

    if (strcmp(argv[1], "e") == 0) {
        assert(argc == 4);
        return embed(argv[2], argv[3]);
    } else {
        assert(argc == 3);
        return extract(argv[2]);
    }
}

int extract(const char* carrierName) {
    struct data* image = read_file(carrierName);
    struct data* output = malloc(sizeof(struct data));

    lsb1_extract(image, output);

    FILE* out = fopen("extracted", "w");
    fwrite(output->bytes, sizeof(unsigned char), output->len, out);
    fclose(out);

    free_data(image);
    free_data(output);

    return 0;
}


int embed(const char* carrierName, const char* inputName) {

    struct data* image = read_file(carrierName);
    struct data* rawInput = read_file(inputName);

    size_t bitCapacity = lsb1_bit_capacity(image->len);

    if (bitCapacity < rawInput->len * 8) {
        printf("The carrier is not big enough. It can only carry %u bytes.\n", bitCapacity / 8);

        free_data(image);
        free_data(rawInput);

        return 1;
    }

    lsb1_embed(image, rawInput);

    FILE* out = fopen("out", "w");
    fwrite(image->bytes, sizeof(unsigned char), image->len, out);
    fclose(out);

    free_data(image);
    free_data(rawInput);

    return 0;
}

