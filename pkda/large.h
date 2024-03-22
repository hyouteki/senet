#ifndef LARGE_H_
#define LARGE_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct Large {
	char *string;
	unsigned int len;
} Large;

static unsigned int max(unsigned int, unsigned int);
static void chop_front(char **, unsigned int);
static unsigned int remove_leading_zeroes(char **, unsigned int);
static void rev_string(char **, unsigned int);

Large Large_new(char *, unsigned int);
Large Large_from_uint(unsigned int num);
int Large_equal(Large, Large);
int Large_greater(Large, Large);
int Large_smaller(Large, Large);
Large Large_add(Large, Large);
Large Large_sub(Large, Large);
Large Large_mul(Large, Large);
Large Large_div(Large, Large);
Large Large_mod(Large, Large);
Large Large_shl(Large, unsigned int);
Large Large_shr(Large, unsigned int);
unsigned int Large_last_digit(Large);
Large Large_mul_mod(Large, Large, Large);
void Large_print(Large);

#define Large_fmt "%.*s"
#define Large_zero (Large) {.string = "0", .len = 1}
#define Large_one (Large) {.string = "1", .len = 1}
#define Large_two (Large) {.string = "2", .len = 1}

#define gen_string(len) ((char *) malloc(sizeof(char)*len))

static unsigned int max(unsigned int n1, unsigned int n2) {
	return (n1 > n2)? n1: n2;
}

static void chop_front(char **string_ptr, unsigned int len) {
	*string_ptr = *string_ptr + sizeof(char)*len;
}

static unsigned int remove_leading_zeroes(char **string, unsigned int len) {
	int i = 0;
	for (; i < len; ++i) if ((*string)[i] != '0') break;
	if (i == len) {
		*string = "0";
		return 1;
	} 
	chop_front(string, i);
	return len-i;
}

static void rev_string(char **string_ptr, unsigned int len) {
	char *string = *string_ptr;
	for (int l = 0, h = len-1; l < h; ++l, --h) {
		string[l] = string[l] ^ string[h];
		string[h] = string[l] ^ string[h];
		string[l] = string[l] ^ string[h];
	}
}

Large Large_new(char *string, unsigned int len) {
	char *new_string = gen_string(len+1);
	new_string = string;
	unsigned int new_len = remove_leading_zeroes(&new_string, len);
	return (Large) {.string = new_string, .len = new_len};
}

Large Large_from_uint(unsigned int num) {
	unsigned int len = 0, tmp = num;
	while (tmp) {
		++len;
		tmp /= 10;
	}
	
	char *string = gen_string(len+1);
    for (int i = len-1, tmp = num; i >= 0; --i, tmp /= 10) {
		string[i] = tmp%10+'0';
	}
	string[len] = '\0';
    return Large_new(string, len);
}

int Large_equal(Large l1, Large l2) {
	if (l1.len != l2.len) return 0;
	for (int i = 0; i < l1.len; ++i) {
		if (l1.string[i] != l2.string[i]) return 0;
	}
	return 1;
}

int Large_greater(Large l1, Large l2) {
	if (l1.len != l2.len) return l1.len > l2.len;
	for (int i = 0; i < l1.len; ++i) {
		if (l1.string[i] != l2.string[i]) return l1.string[i] > l2.string[i];
	}
	return 0;
}

int Large_smaller(Large l1, Large l2) {
	return !(Large_equal(l1, l2) || Large_greater(l1, l2));
}

Large Large_add(Large l1, Large l2) {
	char *string = gen_string(max(l1.len, l2.len)+2);
	unsigned int len = 0;
	
	int i1 = l1.len-1, i2 = l2.len-1, carry = 0;
	while (i1 >= 0 && i2 >= 0) {
		unsigned int sum = l1.string[i1--]-'0' + l2.string[i2--]-'0' + carry;
		carry = sum/10;
		string[len++] = sum%10 + '0';
	}
	while (i1 >= 0) {
		unsigned int sum = l1.string[i1--]-'0' + carry;
		carry = sum/10;
		string[len++] = sum%10 + '0';
	}
	while (i2 >= 0) {
		unsigned int sum = l2.string[i2--]-'0' + carry;
		carry = sum/10;
		string[len++] = sum%10 + '0';
	}
	if (carry != 0) {
		string[len++] = carry + '0';
	}

	rev_string(&string, len);
	string[len] = '\0';
	
	return Large_new(string, len);
}

Large Large_sub(Large l1, Large l2) {
	if (Large_equal(l1, l2)) return Large_zero;
	assert(Large_greater(l1, l2) && "Underflow: l1 < l2");

	char *string = gen_string(l2.len+1);

	int len = 0;
	for (int i = 0; i < l1.len-l2.len; ++i) string[len++] = '9';
	for (int i = 0; i < l2.len; ++i) string[len++] = 9-l2.string[i]+'0'+'0';
	string[len] = '\0';

	Large result = Large_new(string, len);
	result = Large_add(result, Large_one);
	result = Large_add(result, l1);
	if (result.len > l1.len) chop_front(&result.string, 1);
	result.len = remove_leading_zeroes(&result.string, len);
	
	return result;
}

Large Large_mul(Large l1, Large l2) {
	if (Large_equal(l1, Large_zero) || Large_equal(l2, Large_zero))
		return Large_zero;
	
	int partial_products[l1.len+l2.len];
	for (int i = 0; i < l1.len+l2.len; ++i) partial_products[i] = 0;	
	for (int i = l1.len-1; i >= 0; --i) {
		for (int j = l2.len-1; j >= 0; --j) {
			partial_products[l1.len-1+l2.len-1-i-j] +=
				(l1.string[i]-'0')*(l2.string[j]-'0');
		}
	}

	char *string = gen_string(l1.len+l2.len+1);
	unsigned int carry = 0;
	int len = 0;
	for (int i = 0; i < l1.len+l2.len; ++i) {
		int sum = carry + partial_products[i];
		partial_products[i] = sum%10;
		carry = sum/10;
		string[len++] = sum%10 + '0';
	}
	rev_string(&string, len);
	len = remove_leading_zeroes(&string, len);
		
	return Large_new(string, len);
}

Large Large_div(Large l1, Large l2) {
	assert(!Large_equal(l2, Large_zero) && "ZeroDivisionError: l2 = 0");
	if (Large_smaller(l1, l2)) return Large_zero;
	if (Large_equal(l1, l2)) return Large_one;

	int i = 0;
	Large dividend = Large_zero, Large_ten = Large_from_uint(10);
	for (; i < l1.len && Large_smaller(Large_add(Large_mul(dividend, Large_ten), Large_from_uint(l1.string[i]-'0')), l2); ++i) {
		dividend = Large_add(Large_mul(dividend, Large_ten), Large_from_uint(l1.string[i]-'0'));
	}

	char *quotient = gen_string(l1.len+1);
	unsigned int len = 0;
	for (; i < l1.len; ++i) {
		dividend = Large_add(Large_mul(dividend, Large_ten), Large_from_uint(l1.string[i]-'0'));
		int factor = 9;
		for (; Large_greater(Large_mul(Large_from_uint(factor), l2), dividend); --factor);
		dividend = Large_sub(dividend, Large_mul(Large_from_uint(factor), l2));
		quotient[len++] = factor+'0';
	}
	len = remove_leading_zeroes(&quotient, len);
	return Large_new(quotient, len);
}

Large Large_mod(Large l1, Large l2) {
	assert(!Large_equal(l2, Large_zero) && "ZeroDivisionError: l2 = 0");
	if (Large_smaller(l1, l2)) return Large_zero;
	if (Large_equal(l1, l2)) return Large_one;

	int i = 0;
	Large dividend = Large_zero, Large_ten = Large_from_uint(10);
	for (; i < l1.len && Large_smaller(Large_add(Large_mul(dividend, Large_ten), Large_from_uint(l1.string[i]-'0')), l2); ++i) {
		dividend = Large_add(Large_mul(dividend, Large_ten), Large_from_uint(l1.string[i]-'0'));
	}

	unsigned int len = 0;
	for (; i < l1.len; ++i) {
		dividend = Large_add(Large_mul(dividend, Large_ten), Large_from_uint(l1.string[i]-'0'));
		int factor = 9;
		for (; Large_greater(Large_mul(Large_from_uint(factor), l2), dividend); --factor);
		dividend = Large_sub(dividend, Large_mul(Large_from_uint(factor), l2));
	}
	dividend.len = remove_leading_zeroes(&dividend.string, dividend.len);
	return dividend;
}

Large Large_shl(Large l, unsigned int n) {
	Large result = l;
	while (n) {
		result = Large_mul(result, Large_two);
		--n;
	}
	return result;
}

Large Large_shr(Large l, unsigned int n) {
	Large result = l;
	while (n) {
		result = Large_div(result, Large_two);
		--n;
	}
	return result;
}

unsigned int Large_last_digit(Large l) {
	return l.string[l.len-1]-'0';
}

Large Large_mul_mod(Large l1, Large l2, Large mod) {
	Large result = Large_zero;
	while (!Large_equal(l1, Large_zero)) {
		if (Large_last_digit(l1) & 1)
			result = Large_mod(Large_add(result, l2), mod);
		l1 = Large_shr(l1, 1);
		l2 = Large_mod(Large_shl(l2, 1), mod);
	}
	return result;
}

void Large_print(Large l1) {
	printf(Large_fmt"\n", l1.len, l1.string);
}

#endif // LARGE_H_
