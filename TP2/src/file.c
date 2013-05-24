#include <stdio.h>
#include <unistd.h>

#include "file.h"

int fsize(FILE* file);

int fsize(FILE* file) {
    assert(file);

    int cur = ftell(file);
    assert(fseek(file, 0, SEEK_END) == 0);

    int size = ftell(file);
    assert(fseek(file, cur, SEEK_SET) == 0);

    return size;
}

struct data* read_file(const char* filename) {
    assert(filename);

    FILE* file = fopen(filename, "r");
    assert(file);

    struct data* data = malloc(sizeof(struct data));
    assert(data);

    data->len = fsize(file);
    assert(ftell(file) == 0);
    data->bytes = malloc(sizeof(unsigned char) * data->len);

    assert(fread(data->bytes, sizeof(unsigned char), data->len, file) == data->len);

    fclose(file);

    return data;
}

