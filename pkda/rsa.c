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

int main(int argc, char **argv) {
	assert(argc >= 2 && "Error: subcommand not present");
	if (strcmp(argv[1], "genkeys")) {
		mpz_t p, q, n;
		gmp_randstate_t state;
		mpz_init(p);
		mpz_init(q);
		mpz_init(n);
		gmp_randinit_default(state);
		gmp_randseed_ui(state, time(NULL));

		unsigned int bit_size = (argc == 3)? stoul(argv[2]): BIT_SIZE;
		gen_prime_num(p, state, bit_size);
		gen_prime_num(q, state, bit_size);

		mpz_mul(n, p, q);

		gmp_printf("p: %Zd\n\n", p);
		gmp_printf("q: %Zd\n\n", q);
		gmp_printf("n: %Zd\n", n);

		mpz_clear(p);
		mpz_clear(q);
		mpz_clear(n);
		gmp_randclear(state);
	}
    return 0;
}
