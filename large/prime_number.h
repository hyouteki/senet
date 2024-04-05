#ifndef PRIME_NUMBER_H_
#define PRIME_NUMBER_H_

#include <iostream>
#include <vector>
#include "big_number.h"
using namespace std;

#define MILLER_RABIN_TRIAL_COUNT 20

vector<int> first_few_primes = {
	2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
	73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151,
	157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233,
	239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317,
	331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419,
	421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503,
	509, 521, 523, 541};

bool primality_test_with_first_few_primes(big_number &num)
{
	for (auto prime : first_few_primes)
	{
		if (num == prime || num % prime == 0)
			return false;
	}
	return true;
}

bool is_composite(big_number &round_tester, big_number &even_component, big_number &candidate, int trailing_zeroes)
{
	if (power_modulus(round_tester, even_component, candidate) == 1)
		return false;
	for (int i = 0; i < trailing_zeroes; ++i)
	{
		big_number tmp(1);
		tmp.left_shift(i);
		big_number trash;
		trash = tmp * even_component;
		if (power_modulus(round_tester, trash, candidate) == candidate - 1)
			return false;
	}
	return true;
}

bool primality_test_with_miller_rabin(big_number &candidate, int bit_size)
{
	int trailing_zeroes = 0;
	big_number even_component;
	even_component = candidate;
	if (even_component.last_digit() & 1)
		--even_component;
	while ((even_component.last_digit() & 1) == 0)
	{
		even_component.right_shift(1);
		++trailing_zeroes;
	}
	for (int i = 0; i < MILLER_RABIN_TRIAL_COUNT; ++i)
	{
		big_number round_tester = get_random_number(bit_size);
		while (round_tester >= candidate)
		{
			round_tester = get_random_number(bit_size);
		}
		if (is_composite(round_tester, even_component, candidate, trailing_zeroes))
			return false;
	}
	return true;
}

big_number &get_random_prime(int bit_size)
{
	while (true)
	{
		big_number *num = &get_random_number(bit_size);
		if (primality_test_with_first_few_primes(*num))
		{
			if (primality_test_with_miller_rabin(*num, bit_size))
			{
				return *num;
			}
		}
	}
}

#endif // PRIME_NUMBER_H_
