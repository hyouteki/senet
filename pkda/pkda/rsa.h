#ifndef PKDA_RSA_H_
#define PKDA_RSA_H_

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>
#include <assert.h>
#include "utils.h"

#define NUM_ROUNDS 20
#define BIT_SIZE 2048

static void gen_odd_num(mpz_t, gmp_randstate_t, unsigned int);
static void gen_prime_num(mpz_t, gmp_randstate_t, unsigned int);
static unsigned int char_to_num(char);
static char num_to_char(unsigned int);
static char *encrypt_chunk(char *, mpz_t, mpz_t, unsigned int, unsigned int, mpz_t);
static char *decrypt_chunk(char *, mpz_t, mpz_t, unsigned int, unsigned int, mpz_t);

void genkeys(unsigned int, mpz_t, mpz_t, mpz_t);
char *encrypt(char *, mpz_t, mpz_t);
char *decrypt(char *, mpz_t, mpz_t);

static char chars[] = {
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
	'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D',
	'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S',
	'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', ' ', '!', '"', '#', '\n', '%', '&', '\'', '(', ')', '*', '+', ',',
	'-', '.', '/', ':', ';', '=', '[', '\\', ']', '^', '_', '\0', '{', '|', '}'};

#define NUM_CHARS sizeof(chars)/sizeof(char)
#define chunk_size(num) ((mpz_sizeinbase(num, 10) - 2)>>1)

static unsigned int char_to_num(char ch) {
	for (int i = 0; i < NUM_CHARS; ++i) if (chars[i] == ch) return 10+i;
	true_unless_kill(0, "invalid char");
}

static char num_to_char(unsigned int num) {
	unsigned int index = num-10;
	true_unless_kill(index < NUM_CHARS, "invalid num");
	return chars[index];
}

static void gen_odd_num(mpz_t num, gmp_randstate_t state, unsigned int bit_size) {
	mpz_urandomb(num, state, bit_size);
	mpz_setbit(num, 0);
}

static void gen_prime_num(mpz_t prime, gmp_randstate_t state, unsigned int bit_size) {
    while (1) {
        gen_odd_num(prime, state, bit_size);
        if (mpz_probab_prime_p(prime, NUM_ROUNDS)) break;
    }
}

void genkeys(unsigned int bit_size, mpz_t e, mpz_t d, mpz_t n) {
	mpz_t p, q, phi;
	gmp_randstate_t state;
	mpz_inits(p, q, phi, n, e, d, NULL);
	gmp_randinit_default(state);
	gmp_randseed_ui(state, time(NULL));

	gen_prime_num(p, state, bit_size);
	gen_prime_num(q, state, bit_size);
	gmp_randclear(state);

	mpz_mul(n, p, q);
	mpz_sub_ui(p, p, 1);
	mpz_sub_ui(q, q, 1);
	mpz_mul(phi, p, q);
	
	mpz_set_ui(e, 65537);
	mpz_invert(d, e, phi);

	mpz_clears(p, q, phi, NULL);
}

static char *encrypt_chunk(char *plaintext, mpz_t e, mpz_t n, unsigned int l, unsigned int h, mpz_t counter) {
	mpz_t m, c;
	mpz_inits(m, c, NULL);
	mpz_set_ui(m, 0);

	char *ptr = plaintext+l;
	int i = l;
	while (*ptr != 0 && i < h) {
		mpz_mul_ui(m, m, 100);
		mpz_add_ui(m, m, char_to_num(*ptr));
		++ptr;
		++i;
	}
	true_unless_kill(mpz_cmp(m, n) < 0, "plaintext number representation exceeds the n");
	
	mpz_xor(m, m, counter);
	mpz_add_ui(counter, counter, 1);
	
	mpz_powm(c, m, e, n);

	char *ciphertext = (char *) malloc(sizeof(char)*(mpz_sizeinbase(c, 10)+1));
    true_unless_kill(ciphertext != NULL, "failed to allocate memory");
	mpz_get_str(ciphertext, 10, c);

	char *zeroes = (char *) malloc(sizeof(char)*(mpz_sizeinbase(n, 10)+1));
	i = 0;
	for (; i < mpz_sizeinbase(n, 10)-mpz_sizeinbase(c, 10); ++i) zeroes[i] = '0';
	zeroes[i] = 0;
	
	mpz_clears(m, c, NULL);

	return sappend(zeroes, ciphertext);
}

static char *decrypt_chunk(char *ciphertext, mpz_t d, mpz_t n, unsigned int l, unsigned int h, mpz_t counter) {
	mpz_t c, m;
	mpz_inits(c, m, NULL);

	char *chunk = (char *) malloc(sizeof(char)*(h-l+1));
	for (int i = 0; i < h-l; ++i) chunk[i] = ciphertext[i+l];
	chunk[h-l] = 0;
	mpz_set_str(c, chunk, 10);
	true_unless_kill(mpz_cmp(c, n) < 0, "ciphertext number representation exceeds the n");
	
	mpz_powm(m, c, d, n);

	mpz_xor(m, m, counter);
	mpz_add_ui(counter, counter, 1);
	
	char *plaintext_nums = (char *) malloc(sizeof(char)*(mpz_sizeinbase(m, 10)+1));
    true_unless_kill(plaintext_nums != NULL, "failed to allocate memory");
	mpz_get_str(plaintext_nums, 10, m);

	char *plaintext = (char *) malloc(sizeof(char)*((mpz_sizeinbase(m, 10)>>1)+1));
	unsigned int i = 0;
	for (; i < mpz_sizeinbase(m, 10) && plaintext_nums[i]; i += 2) {
		int num = (plaintext_nums[i]-'0')*10+plaintext_nums[i+1]-'0';
		plaintext[i>>1] = num_to_char(num);
	}
	plaintext[i>>1] = 0;
	mpz_clears(m, c, NULL);
	free(plaintext_nums);

	return plaintext;
}

char *encrypt(char *plaintext, mpz_t e, mpz_t n) {
	char *ciphertext = "";
	unsigned int chunk_sz = chunk_size(n), i = 0, k = slen(plaintext)/chunk_sz;
	if (slen(plaintext)%chunk_sz != 0) ++k;
	mpz_t counter;
	mpz_init(counter);
	mpz_set_ui(counter, 0);
	while (i < k && plaintext+i) {
		char *encrypted_chunk = encrypt_chunk(plaintext, e, n, chunk_sz*i, chunk_sz*(i+1), counter);
		ciphertext = sappend(ciphertext, encrypted_chunk);
		++i;
	}
	mpz_clear(counter);
	return ciphertext;
}

char *decrypt(char *ciphertext, mpz_t d, mpz_t n) {
	char *plaintext = "";
	unsigned int chunk_sz = mpz_sizeinbase(n, 10), i = 0, k = slen(ciphertext)/chunk_sz;
	if (slen(ciphertext)%chunk_sz != 0) ++k;
	mpz_t counter;
	mpz_init(counter);
	mpz_set_ui(counter, 0);
	while (i < k && ciphertext+i) {
		char *decrypted_chunk = decrypt_chunk(ciphertext, d, n, chunk_sz*i, chunk_sz*(i+1), counter);
		plaintext = sappend(plaintext, decrypted_chunk);
		++i;
	}
	mpz_clear(counter);
	return plaintext;
}

#endif // PKDA_RSA_H_
