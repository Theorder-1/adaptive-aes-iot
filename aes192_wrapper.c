#define AES192 1
#undef AES128
#undef AES256

#define AES_ctx AES192_ctx

#define AES_init_ctx AES192_init_ctx
#define AES_init_ctx_iv AES192_init_ctx_iv
#define AES_ctx_set_iv AES192_ctx_set_iv

#define AES_ECB_encrypt AES192_ECB_encrypt
#define AES_ECB_decrypt AES192_ECB_decrypt

#define AES_CBC_encrypt_buffer AES192_CBC_encrypt_buffer
#define AES_CBC_decrypt_buffer AES192_CBC_decrypt_buffer

#define AES_CTR_xcrypt_buffer AES192_CTR_xcrypt_buffer

#include "aes_impl.inc"

void AES192_encrypt_block(const unsigned char *key, unsigned char *buffer)
{
    struct AES192_ctx ctx;

    AES192_init_ctx(&ctx, key);
    AES192_ECB_encrypt(&ctx, buffer);
}

void AES192_decrypt_block(const unsigned char *key, unsigned char *buffer)
{
    struct AES192_ctx ctx;

    AES192_init_ctx(&ctx, key);
    AES192_ECB_decrypt(&ctx, buffer);
}