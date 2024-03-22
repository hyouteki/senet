#include <stdio.h>
#include "large.h"
#include "assert.h"

#define assert_large_equal(l, val) (assert(Large_equal(l, Large_from_uint(val))))

int main() {
	Large l1 = Large_new("1000", 4);
	Large l2 = Large_new("448", 3);
	Large l3 = Large_add(l1, l2);
	Large l4 = Large_sub(l1, l1);
	Large l5 = Large_new("0000", 4);
	Large l6 = Large_sub(l1, l2);
	Large l7 = Large_mul(l2, l1);
	Large l8 = Large_from_uint(180);
	Large l9 = Large_div(l1, l2);
	Large l10 = Large_div(Large_from_uint(4568), Large_from_uint(1142));

	assert_large_equal(l1, 1000);
	assert_large_equal(l2, 448);
	assert_large_equal(l3, 1448);
	assert_large_equal(l4, 0);
	assert_large_equal(l5, 0);
	assert_large_equal(l6, 552);
	assert_large_equal(l7, 448000);
	assert_large_equal(l8, 180);
	assert_large_equal(l9, 2);
	assert_large_equal(l10, 4);
}
