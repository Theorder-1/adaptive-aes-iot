#define AES128 1
#undef AES192
#undef AES256

#define AES_ctx AES128_ctx

#define AES_init_ctx AES128_init_ctx
#define AES_init_ctx_iv AES128_init_ctx_iv
#define AES_ctx_set_iv AES128_ctx_set_iv

#define AES_ECB_encrypt AES128_ECB_encrypt
#define AES_ECB_decrypt AES128_ECB_decrypt

#define AES_CBC_encrypt_buffer AES128_CBC_encrypt_buffer
#define AES_CBC_decrypt_buffer AES128_CBC_decrypt_buffer

#define AES_CTR_xcrypt_buffer AES128_CTR_xcrypt_buffer

#include "aes_impl.inc"

void AES128_encrypt_block(const unsigned char *key, unsigned char *buffer)
{
    struct AES128_ctx ctx;

    AES128_init_ctx(&ctx, key);
    AES128_ECB_encrypt(&ctx, buffer);
}

void AES128_decrypt_block(const unsigned char *key, unsigned char *buffer)
{
    struct AES128_ctx ctx;

    AES128_init_ctx(&ctx, key);
    AES128_ECB_decrypt(&ctx, buffer);
}