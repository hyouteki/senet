#ifndef PRIME_H_
#define PRIME_H_

#include <stdio.h>
#include "large.h"

#define MILLER_RABIN_TRIAL_COUNT 13
#define PRIME_ARRAY_LEN 100

static unsigned int first_few_primes[PRIME_ARRAY_LEN] = {
	2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
	73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151,
	157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233,
	239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317,
	331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419,
	421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503,
	509, 521, 523, 541};

static int primality_test_with_first_few_primes(Large);
static int is_composite(Large, Large, Large, unsigned int);
static int primality_test_with_miller_rabin(Large, unsigned int);

Large Prime_get_random(unsigned int);

static int primality_test_with_first_few_primes(Large l) {
	for (int i = 0; i < PRIME_ARRAY_LEN; ++i) {
		Large prime = Large_from_uint(first_few_primes[i]);
		if (Large_equal(l, prime)) return 1;
		if (Large_equal(Large_mod(l, prime), Large_zero)) return 0;
	}
	return 1;
} 

static int is_composite(Large round_tester, Large even_component,
						Large candidate, unsigned int trailing_zeroes) {
	Large l1 = Large_one;
	if (Large_equal(Large_pow_mod(round_tester, even_component, candidate), l1)) return 0;
	
	for (int i = 0; i < trailing_zeroes; ++i) {
		Large tmp = Large_mul(Large_shl(l1, i), even_component);
		if (Large_equal(Large_pow_mod(round_tester, tmp, candidate),
						Large_sub(candidate, l1))) return 0;
	}
	
	return 1;
}

static int primality_test_with_miller_rabin(Large candidate, unsigned int bit_size) {
	unsigned int trailing_zeroes = 0;
	Large even_component = candidate;
	
	if (Large_last_digit(even_component) & 1)
		even_component = Large_sub(even_component, Large_one);
	
	while (Large_last_digit(even_component) & 1 == 0) {
	    Large_shr(even_component, 1);
		++trailing_zeroes;
	}

	for (int i = 0; i < MILLER_RABIN_TRIAL_COUNT; ++i) {
		Large round_tester = Large_random_odd(bit_size);
		while (!Large_smaller(round_tester, candidate))
		    round_tester = Large_random_odd(bit_size);
		if (is_composite(round_tester, even_component, candidate, trailing_zeroes))
			return 0;
	}

	return 1;
}

Large Prime_get_random(unsigned int bit_size) {
	while (1) {
		Large l = Large_random_odd(bit_size);
		if (primality_test_with_first_few_primes(l)) {
			if (primality_test_with_miller_rabin(l, bit_size)) return l;
		}
	}
}

#endif // PRIME_H_
