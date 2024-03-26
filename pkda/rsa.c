#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#define NUM_ROUNDS 20
#define BIT_SIZE 2048

static int strcmp(char *, char *);
static unsigned int stoul(char *);

static void gen_odd_num(mpz_t, gmp_randstate_t, unsigned int);
static void gen_prime_num(mpz_t, gmp_randstate_t, unsigned int);
static void write_key_to_file(mpz_t, mpz_t, char *, char);

static int strcmp(char *a, char *b) {
	char *ptr_a = a, *ptr_b = b;
	while (*ptr_a && *ptr_b) {
		if (*ptr_a != *ptr_b) return 0;
		++ptr_a;
		++ptr_b;
	}
	return *ptr_a == *ptr_b;
}

static unsigned int stoul(char *a) {
	char *ptr = a;
	unsigned int num = 0;
	while (*ptr) {
		num = num*10 + *ptr - '0';
		++ptr;
	}
	return num;
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

int main(int argc, char **argv) {
	assert(argc >= 2 && "Error: subcommand not present");
	if (strcmp(argv[1], "genkeys")) {
		mpz_t p, q, phi, n, e, d;
		gmp_randstate_t state;
		mpz_inits(p, q, phi, n, e, d, NULL);
		gmp_randinit_default(state);
		gmp_randseed_ui(state, time(NULL));

		unsigned int bit_size = (argc == 3)? stoul(argv[2]): BIT_SIZE;
		gen_prime_num(p, state, bit_size);
		gen_prime_num(q, state, bit_size);
		gmp_randclear(state);

		mpz_sub_ui(p, p, 1);
		mpz_sub_ui(q, q, 1);
		mpz_mul(phi, p, q); // phi = (p-1)*(q-1)
		mpz_mul(n, p, q);

		mpz_set_ui(e, 65537); // most common value of e
		mpz_invert(d, e, phi); // e*d mod phi = 1

		write_key_to_file(e, n, "publickey.txt", 'e');
		write_key_to_file(d, n, "privatekey.txt", 'd');
		
		gmp_printf("p: %Zd\n\n", p);
		gmp_printf("q: %Zd\n\n", q);
		gmp_printf("e: %Zd\n\n", e);
		gmp_printf("d: %Zd\n\n", d);
		gmp_printf("n: %Zd\n", n);
		
		mpz_clears(p, q, phi, n, e, d, NULL);
	}
    return 0;
}
