#include "ssh_crypto_common.h"
#include "ssh_cipher.h"
#include <openssl/evp.h>

#define EVP_CIPHER_FUNC(NAME, evp, k, i, b) \
	static SSH_CIPHER* evp_##NAME(int e) { \
		SSH_CIPHER *c = new_ssh_cipher_evp(evp, k, i, b, e); \
		c->name = #NAME; \
		return c; \
	}

EVP_CIPHER_FUNC(aes256_ctr, EVP_aes_256_ctr, 32, 16, 16)
EVP_CIPHER_FUNC(aes192_ctr, EVP_aes_192_ctr, 24, 16, 16)
EVP_CIPHER_FUNC(aes128_ctr, EVP_aes_128_ctr, 16, 16, 16)

static int dummy_init(SSH_CIPHER *me, const uint8_t *key, const uint8_t *IV)
{
	return 1;
}

static void dummy_deinit(SSH_CIPHER *me)
{
}

SSH_CIPHER ssh_cipher_dummy = {
	.name = "(none)",
	.started = false,
	.init = dummy_init,
	.cleanup = dummy_deinit,
};

struct
{
	const char *name;
	NEW_CIPHER f;
} all_ciphers[] = {
	{ "aes256-ctr", evp_aes256_ctr },
	{ "aes192-ctr", evp_aes192_ctr },
	{ "aes128-ctr", evp_aes128_ctr },
	{ NULL, NULL }
};

NEW_CIPHER search_cipher(const char *s)
{
	int i = search_name((name_sp)all_ciphers, s);
	if (i!=-1)
		return all_ciphers[i].f;
	else
		return NULL;
}

const char all_ciphers_list[] = "aes256-ctr,aes192-ctr,aes128-ctr";
