#include "crypt.h"

#include <string.h>

#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/des.h>

const EVP_CIPHER* get_cipher_type(const enum cipher cipher, const enum cipher_mode mode);

size_t get_key_length(const enum cipher cipher);

struct data* do_encrypt(const struct data* in, const enum cipher cipher, const enum cipher_mode mode, const char* password) {

    assert(password);
    assert(in);

    EVP_CIPHER_CTX* ctx = malloc(sizeof(EVP_CIPHER_CTX));
    assert(ctx);

    EVP_CIPHER_CTX_init(ctx);

    size_t keyLength = get_key_length(cipher);

    unsigned char* key = malloc(keyLength + EVP_MAX_IV_LENGTH);
    unsigned char* iv = malloc(keyLength);

    assert(key && iv);

    const EVP_CIPHER* cipherType = get_cipher_type(cipher, mode);

    assert(EVP_BytesToKey(cipherType, EVP_md5(), NULL, (const unsigned char*) password, strlen(password), 1, key, iv));

    size_t bufferLength = in->len + AES_BLOCK_SIZE + EVP_MAX_IV_LENGTH;
    unsigned char* buffer = malloc(bufferLength);
    assert(buffer);

    size_t length = 0;
    int lengthDelta;

    assert(EVP_EncryptInit_ex(ctx, cipherType, NULL, key, iv));

    assert(EVP_EncryptUpdate(ctx, buffer, &lengthDelta, in->bytes, in->len));
    length = lengthDelta;

    assert(EVP_EncryptFinal_ex(ctx, buffer + length, &lengthDelta));

    length += lengthDelta;

    EVP_CIPHER_CTX_cleanup(ctx);

    struct data* out = malloc(sizeof(struct data));

    out->bytes = buffer;
    out->len = length;

    return out;
}

struct data* do_decrypt(const struct data* in, const enum cipher cipher, const enum cipher_mode mode, const char* password) {

    assert(password);
    assert(in);

    EVP_CIPHER_CTX* ctx = malloc(sizeof(EVP_CIPHER_CTX));
    assert(ctx);

    EVP_CIPHER_CTX_init(ctx);

    size_t keyLength = get_key_length(cipher);

    unsigned char* key = malloc(keyLength + EVP_MAX_IV_LENGTH);
    unsigned char* iv = malloc(keyLength);

    assert(key && iv);

    const EVP_CIPHER* cipherType = get_cipher_type(cipher, mode);

    assert(EVP_BytesToKey(cipherType, EVP_md5(), NULL, (const unsigned char*) password, strlen(password), 1, key, iv));

    unsigned char* buffer = malloc(in->len);
    assert(buffer);

    size_t length = 0;
    int lengthDelta;

    assert(EVP_DecryptInit_ex(ctx, cipherType, NULL, key, iv));

    assert(EVP_DecryptUpdate(ctx, buffer, &lengthDelta, in->bytes, in->len));
    length = lengthDelta;

    assert(EVP_DecryptFinal_ex(ctx, buffer + length, &lengthDelta));
    length += lengthDelta;

    EVP_CIPHER_CTX_cleanup(ctx);

    struct data* out = malloc(sizeof(struct data));

    out->bytes = buffer;
    out->len = length;

    return out;
}

size_t get_key_length(const enum cipher cipher) {

    switch (cipher) {
        case CIPHER_AES_256:
            return 256 / 8;
        case CIPHER_AES_192:
            return 192 / 8;
        case CIPHER_AES_128:
        case CIPHER_DES:
            return 128 / 8;
    }

    abort();
}

const EVP_CIPHER* get_cipher_type(const enum cipher cipher, const enum cipher_mode mode) {

    switch (mode) {
        case MODE_ECB:

            switch (cipher) {
                case CIPHER_DES:
                    return EVP_des_ecb();
                case CIPHER_AES_128:
                    return EVP_aes_128_ecb();
                case CIPHER_AES_192:
                    return EVP_aes_192_ecb();
                case CIPHER_AES_256:
                    return EVP_aes_256_ecb();
            }

        case MODE_CBC:

            switch (cipher) {
                case CIPHER_DES:
                    return EVP_des_cbc();
                case CIPHER_AES_128:
                    return EVP_aes_128_cbc();
                case CIPHER_AES_192:
                    return EVP_aes_192_cbc();
                case CIPHER_AES_256:
                    return EVP_aes_256_cbc();
            }

        case MODE_OFB:

            switch (cipher) {
                case CIPHER_DES:
                    return EVP_des_ofb();
                case CIPHER_AES_128:
                    return EVP_aes_128_ofb();
                case CIPHER_AES_192:
                    return EVP_aes_192_ofb();
                case CIPHER_AES_256:
                    return EVP_aes_256_ofb();
            }

        case MODE_CFB:

            switch (cipher) {
                case CIPHER_DES:
                    return EVP_des_cfb();
                case CIPHER_AES_128:
                    return EVP_aes_128_cfb();
                case CIPHER_AES_192:
                    return EVP_aes_192_cfb();
                case CIPHER_AES_256:
                    return EVP_aes_256_cfb();
            }
    }

    abort();
}

