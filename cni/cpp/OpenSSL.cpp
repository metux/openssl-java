#include <gcj/cni.h>
#include <gcj/array.h>
#include <stdio.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#include "de_metux_openssl_OpenSSL.h"

static int handleErrors()
{
	fprintf(stderr, "openssl error: ");
	ERR_print_errors_fp(stderr);
	return -1;
}

static int AES256_do_encrypt(
	unsigned char *plaintext,
	int plaintext_len,
	unsigned char *key,
	unsigned char *iv,
	unsigned char *ciphertext)
{
	EVP_CIPHER_CTX *ctx;

	int len;

	int ciphertext_len;

	/* Create and initialise the context */
	if (!(ctx = EVP_CIPHER_CTX_new()))
		return handleErrors();

	/* Initialise the encryption operation. IMPORTANT - ensure you use a key
	 * and IV size appropriate for your cipher
	 * In this example we are using 256 bit AES (i.e. a 256 bit key). The
	 * IV size for *most* modes is the same as the block size. For AES this
	 * is 128 bits */
	if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
		return handleErrors();

	/* Provide the message to be encrypted, and obtain the encrypted output.
	 * EVP_EncryptUpdate can be called multiple times if necessary
	 */
	if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
		return handleErrors();

	ciphertext_len = len;

	/* Finalise the encryption. Further ciphertext bytes may be written at
	 * this stage.
	 */
	if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
		return handleErrors();

	ciphertext_len += len;

	/* Clean up */
	EVP_CIPHER_CTX_free(ctx);

	return ciphertext_len;
}

static int AES256_do_decrypt(
	unsigned char *ciphertext,
	int ciphertext_len,
	unsigned char *key,
	unsigned char *iv,
	unsigned char *plaintext)
{
	EVP_CIPHER_CTX *ctx;

	int len;

	int plaintext_len;

	/* Create and initialise the context */
	if (!(ctx = EVP_CIPHER_CTX_new()))
		return handleErrors();

	/* Initialise the decryption operation. IMPORTANT - ensure you use a key
	 * and IV size appropriate for your cipher
	 * In this example we are using 256 bit AES (i.e. a 256 bit key). The
	 * IV size for *most* modes is the same as the block size. For AES this
	 * is 128 bits */
	if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
		return handleErrors();

	/* Provide the message to be decrypted, and obtain the plaintext output.
	 * EVP_DecryptUpdate can be called multiple times if necessary
	 */
	if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
		return handleErrors();

	plaintext_len = len;

	/* Finalise the decryption. Further plaintext bytes may be written at
	 * this stage.
	 */
	if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
		return handleErrors();

	plaintext_len += len;

	/* Clean up */
	EVP_CIPHER_CTX_free(ctx);

	return plaintext_len;
}

JArray< jbyte > * de::metux::openssl::OpenSSL::AES256_encrypt(JArray< jbyte > * key, JArray< jbyte > * content) {

	/** hmm, really a good idea ? **/
	unsigned char buffer[65535];
	int sz = AES256_do_encrypt(
		(unsigned char*)elements(content),	/** plaintext **/
		content->length,
		(unsigned char*)elements(key),		/** key **/
		(unsigned char*)elements(key),		/** IV **/
		buffer
	);

	JArray< jbyte >* result = JvNewByteArray(sz);
	memcpy(elements(result), buffer, sz);

	return result;
}

JArray< jbyte > * de::metux::openssl::OpenSSL::AES256_decrypt(JArray< jbyte > * key, JArray< jbyte > * content) {

	/** hmm, really a good idea ? **/
	unsigned char buffer[65535];
	int sz = AES256_do_decrypt(
		(unsigned char*)elements(content),	/** plaintext **/
		content->length,
		(unsigned char*)elements(key),		/** key **/
		(unsigned char*)elements(key),		/** IV **/
		buffer
	);

	JArray< jbyte >* result = JvNewByteArray(sz);
	memcpy(elements(result), buffer, sz);

	return result;
}
