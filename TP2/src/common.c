#include "common.h"

void free_data(struct data* data) {

    assert(data);

    if (data->bytes) {
        free(data->bytes);
        data->bytes = NULL;
    }

    free(data);
}
