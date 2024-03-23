#include <stdio.h>
#include "large.h"
#include "assert.h"

#define assert_large_equal(l, val) (assert(Large_equal(l, Large_from_uint(val))))

int main() {
	Large l1 = Large_new("1000", 4);
	assert_large_equal(l1, 1000);
	
	Large l2 = Large_new("448", 3);
	assert_large_equal(l2, 448);
	
	Large l3 = Large_add(l1, l2);
	assert_large_equal(l3, 1448);
		
	Large l4 = Large_sub(l1, l1);
	assert_large_equal(l4, 0);
		
	Large l5 = Large_new("0000", 4);
	assert_large_equal(l5, 0);
	
	Large l6 = Large_sub(l1, l2);
	assert_large_equal(l6, 552);
	
	Large l7 = Large_mul(l2, l1);
	assert_large_equal(l7, 448000);
	
	Large l8 = Large_from_uint(180);
	assert_large_equal(l8, 180);
	
	Large l9 = Large_div(l1, l2);
	assert_large_equal(l9, 2);
	
	Large l10 = Large_div(Large_from_uint(4568), Large_from_uint(1142));
	assert_large_equal(l10, 4);
	
	Large l11 = Large_shl(Large_from_uint(32), 4);
	assert_large_equal(l11, 512);
	
	Large l12 = Large_shr(l2, 5);
	assert_large_equal(l12, 14);
	assert_large_equal(l2, 448);
		
	Large l13 = Large_mod(l2, Large_from_uint(147));
	assert_large_equal(l13, 7);

	assert(Large_last_digit(l2) == 8);

	assert_large_equal(l1, 1000);
	assert_large_equal(l2, 448);
	Large l14 = Large_mul_mod(l1, l2, Large_from_uint(147));
	assert_large_equal(l1, 1000);
	assert_large_equal(l2, 448);
	assert_large_equal(l14, 91);

	l14 = Large_add(l14, Large_from_uint(9));
	assert_large_equal(l1, 1000);
	assert_large_equal(l14, 100);

	Large l15 = Large_pow_mod(Large_from_uint(3), Large_from_uint(9), Large_from_uint(845));
	assert_large_equal(l15, 248);

	Large l16 = Large_pow(Large_from_uint(3), 12);
	assert_large_equal(l16, 531441);

	assert_large_equal(Large_mod(Large_random_odd(78), Large_two), 1);

	Large l17 = l16;
	l17 = Large_add(l17, Large_one);
	assert_large_equal(l16, 531441);
	assert_large_equal(l17, 531442);
}
