#ifndef AES192_H
#define AES192_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// AES-192 context structure
struct AES192_ctx {
    uint8_t RoundKey[208];
    uint8_t Iv[16];
};

// AES-192 functions
void AES192_init_ctx(struct AES192_ctx* ctx, const uint8_t* key);
void AES192_init_ctx_iv(struct AES192_ctx* ctx, const uint8_t* key, const uint8_t* iv);
void AES192_ctx_set_iv(struct AES192_ctx* ctx, const uint8_t* iv);
void AES192_ECB_encrypt(const struct AES192_ctx* ctx, uint8_t* buf);
void AES192_ECB_decrypt(const struct AES192_ctx* ctx, uint8_t* buf);
void AES192_CBC_encrypt_buffer(struct AES192_ctx* ctx, uint8_t* buf, size_t length);
void AES192_CBC_decrypt_buffer(struct AES192_ctx* ctx, uint8_t* buf, size_t length);
void AES192_CTR_xcrypt_buffer(struct AES192_ctx* ctx, uint8_t* buf, size_t length);

// Simple block-level wrappers
void AES192_encrypt_block(const unsigned char *key, unsigned char *buffer);
void AES192_decrypt_block(const unsigned char *key, unsigned char *buffer);

#ifdef __cplusplus
}
#endif

#endif // AES192_H