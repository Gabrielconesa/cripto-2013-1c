#ifndef __crypt__
#define __crypt__

#include "common.h"

enum cipher {
    CIPHER_AES_128,
    CIPHER_AES_192,
    CIPHER_AES_256,
    CIPER_DES
};

enum cipher_mode {
    MODE_ECB,
    MODE_CFB,
    MODE_OFB,
    MODE_CBC
};

struct data* encrypt(const struct data* in, const enum cipher cipher, const enum cipher_mode mode, const char* password);

struct data* decrypt(const struct data* in, const enum cipher cipher, const enum cipher_mode mode, const char* password);

#endif
