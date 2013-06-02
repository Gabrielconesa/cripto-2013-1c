#include <stdio.h>
#include <string.h>
#include <getopt.h>

#include "lsb.h"
#include "common.h"
#include "file.h"
#include "lsbe.h"
#include "crypt.h"

static const char* short_opts = "p:a:m:";

static const struct option opts[] = {
    { "embed", no_argument, NULL, 'e' },
    { "in", required_argument, NULL, 'I' },
    { "out", required_argument, NULL, 'O' },
    { "steg", required_argument, NULL, 's' },
    { "pass", required_argument, NULL, 'P' },
    { "extract", no_argument, NULL, 'x' },
    { NULL, no_argument, NULL, 0 }
};

#define opt_error(str) { printf(str "\n"); abort(); }

int main(int argc, char *argv[]) {


    const char* carrierName = NULL;
    const char* inputName = NULL;
    const char* outputName = NULL;
    const char* steg = NULL;
    const char* pass = NULL;
    const char* cipherOption = "aes128";
    const char* modeOption = "cbc";
    int op = -1;

    int res;
    while ((res = getopt_long_only(argc, argv, short_opts, opts, NULL)) != -1) {

        switch (res) {
            case 'p':
                carrierName = optarg;
                break;
            case 'a':
                cipherOption = optarg;
                break;
            case 'm':
                modeOption = optarg;
                break;
            case 'e':
                op = 0;
                break;
            case 'x':
                op = 1;
                break;
            case 'I':
                inputName = optarg;
                break;
            case 'O':
                outputName = optarg;
                break;
            case 's':
                steg = optarg;
                break;
            case 'P':
                pass = optarg;
                break;
            default:
                opt_error("Invalid parameter");
        }

    }

    int n;
    if (steg == NULL) {
        opt_error("You must specify an algorithm");
    } else if (strcmp(steg, "LSB1") == 0) {
        n = 1;
    } else if (strcmp(steg, "LSB4") == 0) {
        n = 4;
    } else if (strcmp(steg, "LSBE") == 0) {
        n = 0;
    } else {
        opt_error("You must specify a valid algorithm");
    }

    if (carrierName == NULL || !can_access(carrierName)) {
        opt_error("Can't open carrier file");
    }

    if (outputName == NULL || strlen(outputName) == 0) {
        opt_error("Output file name must be present.");
    }

    enum cipher cipher;
    if (strcmp(cipherOption, "aes128") == 0) {
        cipher = CIPHER_AES_128;
    } else if (strcmp(cipherOption, "aes192") == 0) {
        cipher = CIPHER_AES_192;
    } else if (strcmp(cipherOption, "aes256") == 0) {
        cipher = CIPHER_AES_256;
    } else if (strcmp(cipherOption, "des") == 0) {
        cipher = CIPHER_DES;
    } else {
        opt_error("Invalid cipher algorithm value");
    }

    enum cipher_mode mode;
    if (strcmp(modeOption, "cbc") == 0) {
        mode = MODE_CBC;
    } else if (strcmp(modeOption, "ofb") == 0) {
        mode = MODE_OFB;
    } else if (strcmp(modeOption, "ecb") == 0) {
        mode = MODE_ECB;
    } else if (strcmp(modeOption, "cfb") == 0) {
        mode = MODE_CFB;
    } else {
        opt_error("Invalid cipher chain mode value");
    }

    if (op == -1) {
        opt_error("You must specify whether to extract or embed");
    } else if (op == 1) {
        return extract(n, carrierName, outputName, pass, cipher, mode);
    } else {

        if (inputName == NULL || !can_access(inputName)) {
            opt_error("Can't open input file");
        }

        return embed(n, carrierName, inputName, outputName, pass, cipher, mode);
    }
}

int extract(size_t n, const char* carrierName, const char* outputName, const char* password, const enum cipher cipher, const enum cipher_mode mode) {

    struct data* image = read_file(carrierName);
    struct data* rawOutput = malloc(sizeof(struct data));

    if (n == 0) {
        lsbe_extract(image, rawOutput);
    } else {
        lsb_extract(image, rawOutput, n);
    }

    struct data* output;
    if (password) {
        output = do_decrypt(unpack_data(rawOutput), cipher, mode, password);
        free_data(rawOutput);
    } else {
        output = rawOutput;
    }

    size_t extractedSize = (output->bytes[0] << 24) + (output->bytes[1] << 16) + (output->bytes[2] << 8) + output->bytes[3];

    char* extension = (char*) (output->bytes + sizeof(size_t) + extractedSize);
    char* filename = malloc(sizeof(char) * (strlen(outputName) + strlen(extension) + 1));
    strcpy(filename, outputName);
    strcat(filename, extension);

    FILE* out = fopen(filename, "w");
    fwrite(output->bytes + sizeof(size_t), sizeof(unsigned char), extractedSize, out);
    fclose(out);

    free_data(image);
    free_data(output);

    return 0;
}


int embed(size_t n, const char* carrierName, const char* inputName, const char* outputName, const char* password, const enum cipher cipher, const enum cipher_mode mode) {

    struct data* image = read_file(carrierName);
    struct data* rawInput = read_file(inputName);

    char* extension = strrchr(inputName, '.');
    prepare_data(rawInput, extension);

    struct data* input;
    if (password) {
        input = do_encrypt(rawInput, cipher, mode, password);
        prepare_data(input, NULL);
        free_data(rawInput);
    } else {
        input = rawInput;
    }


    size_t bitCapacity;
    if (n == 0) {
        bitCapacity = lsbe_bit_capacity(image);
    } else {
        bitCapacity = lsb_bit_capacity(image->len, n);
    }

    if (bitCapacity < input->len * 8) {
        printf("The carrier is not big enough. It can only carry %u bytes.\n", bitCapacity / 8);

        free_data(image);
        free_data(input);

        return 1;
    }

    if (n == 0) {
        lsbe_embed(image, input);
    } else {
        lsb_embed(image, input, n);
    }

    FILE* out = fopen(outputName, "w");
    fwrite(image->bytes, sizeof(unsigned char), image->len, out);
    fclose(out);

    free_data(image);
    free_data(input);

    return 0;
}

