#include <stdio.h>
#include <string.h>

#include "lsb.h"
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

    lsb_extract(image, output, 1);

    size_t extractedSize = *((size_t*) output->bytes);

    char* extension = (char*) (output->bytes + sizeof(size_t) + extractedSize);
    char* filename = malloc(sizeof(char) * (strlen("extracted.") + strlen(extension) + 1));
    strcpy(filename, "extracted.");
    strcat(filename, extension);

    FILE* out = fopen(filename, "w");
    fwrite(output->bytes + sizeof(size_t), sizeof(unsigned char), extractedSize, out);
    fclose(out);

    free_data(image);
    free_data(output);

    return 0;
}


int embed(const char* carrierName, const char* inputName) {

    struct data* image = read_file(carrierName);
    struct data* rawInput = read_file(inputName);

    char* extension = strrchr(inputName, '.') + 1;
    prepare_data(rawInput, extension);

    size_t bitCapacity = lsb_bit_capacity(image->len, 1);

    if (bitCapacity < rawInput->len * 8) {
        printf("The carrier is not big enough. It can only carry %u bytes.\n", bitCapacity / 8);

        free_data(image);
        free_data(rawInput);

        return 1;
    }

    lsb_embed(image, rawInput, 1);

    FILE* out = fopen("out", "w");
    fwrite(image->bytes, sizeof(unsigned char), image->len, out);
    fclose(out);

    free_data(image);
    free_data(rawInput);

    return 0;
}

