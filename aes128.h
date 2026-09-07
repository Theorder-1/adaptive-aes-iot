#ifndef AES128_H
#define AES128_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// AES-128 context structure
struct AES128_ctx {
    uint8_t RoundKey[176];
    uint8_t Iv[16];
};

// AES-128 functions
void AES128_init_ctx(struct AES128_ctx* ctx, const uint8_t* key);
void AES128_init_ctx_iv(struct AES128_ctx* ctx, const uint8_t* key, const uint8_t* iv);
void AES128_ctx_set_iv(struct AES128_ctx* ctx, const uint8_t* iv);
void AES128_ECB_encrypt(const struct AES128_ctx* ctx, uint8_t* buf);
void AES128_ECB_decrypt(const struct AES128_ctx* ctx, uint8_t* buf);
void AES128_CBC_encrypt_buffer(struct AES128_ctx* ctx, uint8_t* buf, size_t length);
void AES128_CBC_decrypt_buffer(struct AES128_ctx* ctx, uint8_t* buf, size_t length);
void AES128_CTR_xcrypt_buffer(struct AES128_ctx* ctx, uint8_t* buf, size_t length);

// Simple block-level wrappers
void AES128_encrypt_block(const unsigned char *key, unsigned char *buffer);
void AES128_decrypt_block(const unsigned char *key, unsigned char *buffer);

#ifdef __cplusplus
}
#endif

#endif // AES128_H