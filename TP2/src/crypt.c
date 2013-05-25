#include "crypt.h"

#include <string.h>

#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/des.h>

struct data* encrypt(const struct data* in, const enum cipher cipher, const enum cipher_mode mode, const char* password) {

    assert(cipher == CIPHER_AES_256 && mode == MODE_CBC);
    assert(password);
    assert(in);

    EVP_CIPHER_CTX* ctx = malloc(sizeof(EVP_CIPHER_CTX));
    assert(ctx);

    EVP_CIPHER_CTX_init(ctx);

    // TODO: Determine from cipher
    size_t keyLength = 256;

    unsigned char* key = malloc(keyLength + EVP_MAX_IV_LENGTH);
    unsigned char* iv = malloc(keyLength);

    assert(key && iv);
    // TODO: Unwire cipher
    assert(EVP_BytesToKey(EVP_aes_256_cbc(), EVP_md5(), NULL, (const unsigned char*) password, strlen(password), 5, key, iv));

    size_t bufferLength = in->len + AES_BLOCK_SIZE + EVP_MAX_IV_LENGTH;
    unsigned char* buffer = malloc(bufferLength);
    assert(buffer);

    size_t length = 0;
    int lengthDelta;

    // TODO: Unwire cipher
    assert(EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv));

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

struct data* decrypt(const struct data* in, const enum cipher cipher, const enum cipher_mode mode, const char* password) {

    assert(cipher == CIPHER_AES_256 && mode == MODE_CBC);
    assert(password);
    assert(in);

    EVP_CIPHER_CTX* ctx = malloc(sizeof(EVP_CIPHER_CTX));
    assert(ctx);

    EVP_CIPHER_CTX_init(ctx);

    // TODO: Determine from cipher
    size_t keyLength = 256;

    unsigned char* key = malloc(keyLength + EVP_MAX_IV_LENGTH);
    unsigned char* iv = malloc(keyLength);

    assert(key && iv);
    // TODO: Unwire cipher
    assert(EVP_BytesToKey(EVP_aes_256_cbc(), EVP_md5(), NULL, (const unsigned char*) password, strlen(password), 5, key, iv));

    unsigned char* buffer = malloc(in->len);
    assert(buffer);

    size_t length = 0;
    int lengthDelta;

    // TODO: Unwire cipher
    assert(EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv));

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


