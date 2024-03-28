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
static void write_key_to_file(mpz_t, mpz_t, char *, char);
static unsigned int char_to_num(char);
static char num_to_char(unsigned int);
static unsigned int chunk_size(mpz_t);
static char *encrypt_chunk(char *, mpz_t, mpz_t, unsigned int, unsigned int);

void genkeys(unsigned int, mpz_t, mpz_t, mpz_t);
char *encrypt(char *, mpz_t, mpz_t);

static char chars[] = {
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
	'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D',
	'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S',
	'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', ' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',',
	'-', '.', '/', ':', ';', '<', '=', '>', '?', '@', '[', '\\', ']', '^', '_',
	'\0', '{', '|', '}'};

static unsigned int nums[] = {
	10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
	29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
	48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66,
	67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85,
	86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99};

#define NUM_CHARS sizeof(chars)/sizeof(char)

static unsigned int char_to_num(char ch) {
	for (int i = 0; i < NUM_CHARS; ++i) if (chars[i] == ch) return nums[i];
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

static void write_key_to_file(mpz_t key, mpz_t n, char *filename, char key_prefix) {
	FILE *file = fopen(filename, "w");
	assert(file != NULL && "Error: could not open file");

	char *key_str = mpz_get_str(NULL, 10, key);
	char *n_str = mpz_get_str(NULL, 10, n);
	if (!key_str || !n_str) {
		perror("Error: could not convert mpz to string");
		fclose(file);
		return;
	}

	fprintf(file, "%c:%s\n", key_prefix, key_str);
	fprintf(file, "n:%s\n", n_str);
	free(file);
	fclose(file);
}

static unsigned int chunk_size(mpz_t num) {
	unsigned int chunk_sz = mpz_sizeinbase(num, 10);
	chunk_sz -= 2;
	return chunk_sz>>1;
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

static char *encrypt_chunk(char *plaintext, mpz_t e, mpz_t n, unsigned int l, unsigned int h) {
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
	true_unless_kill(mpz_cmp(m, n) < 0, "plaintext number represenation exceeds the n");
	mpz_powm(c, m, e, n);

	char *ciphertext = (char *) malloc(sizeof(char)*(mpz_sizeinbase(c, 10) + 2));
    true_unless_kill(ciphertext != NULL, "failed to allocate memory");

	mpz_get_str(ciphertext, 10, c);
	mpz_clears(m, c, NULL);

	return ciphertext;
}

char *encrypt(char *plaintext, mpz_t e, mpz_t n) {
	unsigned int chunk_sz = chunk_size(n);
	char *ciphertext = "";
	unsigned int i = 0, k = slen(plaintext)/chunk_sz;
	do {
		char *encrypted_chunk = encrypt_chunk(plaintext, e, n, chunk_sz*i, chunk_sz*(i+1));
		ciphertext = sappend(ciphertext, encrypted_chunk);
		++i;
	} while (i < k && plaintext+i);
	return ciphertext;
}
