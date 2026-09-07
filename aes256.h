#ifndef AES256_H
#define AES256_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// AES-256 context structure
struct AES256_ctx {
    uint8_t RoundKey[240];
    uint8_t Iv[16];
};

// AES-256 functions
void AES256_init_ctx(struct AES256_ctx* ctx, const uint8_t* key);
void AES256_init_ctx_iv(struct AES256_ctx* ctx, const uint8_t* key, const uint8_t* iv);
void AES256_ctx_set_iv(struct AES256_ctx* ctx, const uint8_t* iv);
void AES256_ECB_encrypt(const struct AES256_ctx* ctx, uint8_t* buf);
void AES256_ECB_decrypt(const struct AES256_ctx* ctx, uint8_t* buf);
void AES256_CBC_encrypt_buffer(struct AES256_ctx* ctx, uint8_t* buf, size_t length);
void AES256_CBC_decrypt_buffer(struct AES256_ctx* ctx, uint8_t* buf, size_t length);
void AES256_CTR_xcrypt_buffer(struct AES256_ctx* ctx, uint8_t* buf, size_t length);

// Simple block-level wrappers
void AES256_encrypt_block(const unsigned char *key, unsigned char *buffer);
void AES256_decrypt_block(const unsigned char *key, unsigned char *buffer);

#ifdef __cplusplus
}
#endif

#endif 