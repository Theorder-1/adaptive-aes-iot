#ifndef AESWRAPPER_H
#define AESWRAPPER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void AES128_encrypt_block(
    const unsigned char *key,
    unsigned char *buffer
);

void AES128_decrypt_block(
    const unsigned char *key,
    unsigned char *buffer
);

void AES192_encrypt_block(
    const unsigned char *key,
    unsigned char *buffer
);

void AES192_decrypt_block(
    const unsigned char *key,
    unsigned char *buffer
);

void AES256_encrypt_block(
    const unsigned char *key,
    unsigned char *buffer
);

void AES256_decrypt_block(
    const unsigned char *key,
    unsigned char *buffer
);

#ifdef __cplusplus
}
#endif

#endif