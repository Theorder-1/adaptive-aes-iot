#define AES256 1
#undef AES128
#undef AES192

#define AES_ctx AES256_ctx

#define AES_init_ctx AES256_init_ctx
#define AES_init_ctx_iv AES256_init_ctx_iv
#define AES_ctx_set_iv AES256_ctx_set_iv

#define AES_ECB_encrypt AES256_ECB_encrypt
#define AES_ECB_decrypt AES256_ECB_decrypt

#define AES_CBC_encrypt_buffer AES256_CBC_encrypt_buffer
#define AES_CBC_decrypt_buffer AES256_CBC_decrypt_buffer

#define AES_CTR_xcrypt_buffer AES256_CTR_xcrypt_buffer

#include "aes_impl.inc"

void AES256_encrypt_block(const unsigned char *key, unsigned char *buffer)
{
    struct AES256_ctx ctx;

    AES256_init_ctx(&ctx, key);
    AES256_ECB_encrypt(&ctx, buffer);
}

void AES256_decrypt_block(const unsigned char *key, unsigned char *buffer)
{
    struct AES256_ctx ctx;

    AES256_init_ctx(&ctx, key);
    AES256_ECB_decrypt(&ctx, buffer);
}