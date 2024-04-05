#ifndef BIG_NUMBER_H
#define BIG_NUMBER_H

#include <iostream>
#include <cstring>
#include <vector>
#include <random>
using namespace std;

class big_number
{
	string number;

public:
	// construtors
	big_number(int num = 0);
	big_number(string);
	big_number(const char *);
	big_number(big_number &);

	// assignment operator
	big_number &operator=(const big_number &);

	// methods
	int size() const;
	bool is_null() const;
	void remove_leading_zeroes();
	int last_digit();
	void left_shift(int);
	void right_shift(int);

	// helper methods
	int operator[](const int) const;

	// comparison operators
	friend bool operator==(const big_number &, const big_number &);
	friend bool operator!=(const big_number &, const big_number &);
	friend bool operator>(const big_number &, const big_number &);
	friend bool operator>=(const big_number &, const big_number &);
	friend bool operator<(const big_number &, const big_number &);
	friend bool operator<=(const big_number &, const big_number &);

	// operator overloading for addition and substraction
	friend big_number &operator+=(big_number &, const big_number &);
	friend big_number operator+(const big_number &, const big_number &);
	friend big_number &operator-=(big_number &, const big_number &);
	friend big_number operator-(const big_number &, const big_number &);

	// increment/decrement operator
	big_number &operator++();
	big_number operator++(int);
	big_number &operator--();
	big_number operator--(int);

	// operator overloading for multiplication and division
	friend big_number &operator*=(big_number &, const big_number &);
	friend big_number operator*(const big_number &, const big_number &);
	friend big_number &operator/=(big_number &, const big_number &);
	friend big_number operator/(const big_number &, const big_number &);

	// i/o operators
	friend ostream &operator<<(ostream &, const big_number &);
	friend istream &operator>>(istream &, big_number &);

	// miscellaneous methods
	friend big_number &power(big_number &, int);
	friend big_number &sqrt(big_number &);
	friend big_number &multiply_modulus(big_number &, big_number &, big_number &);
	friend big_number &power_modulus(big_number &, big_number &, big_number &);
	friend big_number &get_random_number(int);
	friend big_number &gcd(big_number &, big_number &);
	friend big_number &min(big_number &, big_number &);
	friend big_number &max(big_number &, big_number &);

	// operator overloading for modulus
	friend big_number &operator%=(big_number &, const big_number &);
	friend big_number operator%(const big_number &, const big_number &);
};

int big_number::size() const
{
	return number.size();
}

bool big_number::is_null() const
{
	return (size() == 1 && number[0] == '0');
}

void big_number::remove_leading_zeroes()
{
	int leading_zero_count = 0;
	while (number[leading_zero_count] == '0')
	{
		++leading_zero_count;
	}
	number = number.substr(leading_zero_count);
	if (number == "")
		number = "0";
}

int big_number::last_digit()
{
	return number[number.size() - 1] - '0';
}

void big_number::left_shift(int num)
{
	while (num)
	{
		(*this) *= 2;
		--num;
	}
}

void big_number::right_shift(int num)
{
	while (num)
	{
		(*this) /= 2;
		--num;
	}
}

int big_number::operator[](const int index) const
{
	if (size() <= index || index < 0)
		throw("ERROR :: Invalid index");
	return number[index];
}

big_number::big_number(int num)
{
	number = "";
	while (num)
	{
		number.insert(0, 1, '0' + num % 10);
		num /= 10;
	}
	if (number == "")
		number = "0";
}

big_number::big_number(string num)
{
	number = "";
	for (char digit : num)
	{
		if (!isdigit(digit))
			throw("ERROR :: Not a number");
		if (digit == '0' && number == "")
			continue;
		number.push_back(digit);
	}
	if (number == "")
		number = "0";
}

big_number::big_number(const char *num)
{
	number = "";
	for (unsigned int i = 0; i < strlen(num); ++i)
	{
		if (!isdigit(num[i]))
			throw("ERROR :: Not a number");
		if (num[i] == '0' && number == "")
			continue;
		number.push_back(num[i]);
	}
	if (number == "")
		number = "0";
}

big_number::big_number(big_number &num)
{
	number = num.number;
}

big_number &big_number::operator=(const big_number &num)
{
	number = num.number;
	return *this;
}

bool operator==(const big_number &num1, const big_number &num2)
{
	return num1.number == num2.number;
}

bool operator!=(const big_number &num1, const big_number &num2)
{
	return !(num1 == num2);
}

bool operator>(const big_number &num1, const big_number &num2)
{
	int n1 = num1.size(), n2 = num2.size();
	if (n1 != n2)
		return n1 > n2;
	for (int i = 0; i < n1; ++i)
	{
		if (num1.number[i] != num2.number[i])
			return num1.number[i] > num2.number[i];
	}
	return false;
}

bool operator<(const big_number &num1, const big_number &num2)
{
	return num2 > num1;
}

bool operator>=(const big_number &num1, const big_number &num2)
{
	return !(num1 < num2);
}

bool operator<=(const big_number &num1, const big_number &num2)
{
	return !(num1 > num2);
}

big_number &operator+=(big_number &num1, const big_number &num2)
{
	int n1 = num1.size(), n2 = num2.size();
	int ptr1 = n1 - 1, ptr2 = n2 - 1;
	int carry = 0;
	string res = "";
	// ripple-carry adder algorithm
	while (ptr1 >= 0 && ptr2 >= 0)
	{
		int sum = (num1.number[ptr1--] - '0') + (num2.number[ptr2--] - '0') + carry;
		res.insert(0, 1, sum % 10 + '0');
		carry = sum / 10;
	}
	while (ptr1 >= 0)
	{
		int sum = (num1.number[ptr1--] - '0') + carry;
		res.insert(0, 1, sum % 10 + '0');
		carry = sum / 10;
	}
	while (ptr2 >= 0)
	{
		int sum = (num2.number[ptr2--] - '0') + carry;
		res.insert(0, 1, sum % 10 + '0');
		carry = sum / 10;
	}
	if (carry)
		res.insert(0, 1, '1');
	num1.number = res;
	num1.remove_leading_zeroes();
	return num1;
}

big_number operator+(const big_number &num1, const big_number &num2)
{
	big_number res(num1.number);
	res += num2;
	return res;
}

big_number &operator-=(big_number &num1, const big_number &num2)
{
	if (num1 < num2)
		throw("ERROR :: Underflow");
	if (num1 == num2)
	{
		num1.number = "0";
		return num1;
	}
	int n1 = num1.size(), n2 = num2.size();
	// substraction using 10's compliment algorithm
	string compliment = "";
	for (int i = 0; i < n1 - n2; ++i)
	{
		compliment.push_back('9');
	}
	for (char ch : num2.number)
		compliment.push_back(9 - (ch - '0') + '0');
	big_number tmp(compliment);
	tmp += 1;
	num1 += tmp;
	if (num1.size() > n1)
	{
		num1.number = num1.number.substr(1);
	}
	num1.remove_leading_zeroes();
	return num1;
}

big_number operator-(const big_number &num1, const big_number &num2)
{
	big_number res(num1.number);
	res -= num2;
	return res;
}

big_number &big_number::operator++()
{
	int i, n = number.size();
	for (i = n - 1; i >= 0 && number[i] == '9'; --i)
		number[i] = '0';
	if (i < 0)
		number.insert(0, 1, '1');
	else
		++number[i];
	return *this;
}

big_number big_number::operator++(int dump)
{
	big_number res(*this);
	++(*this);
	return res;
}

big_number &big_number::operator--()
{
	int i, n = number.size();
	if (n == 1 && number[0] == '0')
		throw("UNDERFLOW");
	for (i = n - 1; number[i] == '0' && i >= 0; --i)
		number[i] = '9';
	--number[i];
	if (n > 1 && number[0] == '0')
		number = number.substr(1);
	return *this;
}

big_number big_number::operator--(int dump)
{
	big_number res(*this);
	--(*this);
	return res;
}

big_number &operator*=(big_number &num1, const big_number &num2)
{
	if (num1.is_null() || num2.is_null())
	{
		num1.number = "0";
		return num1;
	}
	// partial-product ripple-carry algorithm
	int n1 = num1.size(), n2 = num2.size();
	vector<int> partial_products(n1 + n2, 0);
	for (int i = n1 - 1; i >= 0; --i)
	{
		for (int j = n2 - 1; j >= 0; --j)
		{
			partial_products[n1 - 1 - i + n2 - 1 - j] += (num1[i] - '0') * (num2[j] - '0');
		}
	}
	num1.number = "";
	int carry = 0;
	for (int i = 0; i < n1 + n2; i++)
	{
		int sum = carry + partial_products[i];
		partial_products[i] = sum % 10;
		carry = sum / 10;
		num1.number.insert(0, 1, partial_products[i] + '0');
	}
	num1.remove_leading_zeroes();
	return num1;
}

big_number operator*(const big_number &num1, const big_number &num2)
{
	big_number res(num1.number);
	res *= num2;
	return res;
}

big_number &operator/=(big_number &num1, const big_number &num2)
{
	if (num2.is_null())
		throw("ERROR :: Zero division error");
	if (num1 < num2)
	{
		num1.number = "0";
		return num1;
	}
	if (num1 == num2)
	{
		num1.number = "1";
		return num1;
	}
	int i, n1 = num1.size();
	string quotient = "";
	big_number dividend;
	for (i = 0; i < n1 && dividend * 10 + num1[i] - '0' < num2; ++i)
	{
		dividend = dividend * 10 + num1[i] - '0';
	}
	for (; i < n1; ++i)
	{
		dividend = dividend * 10 + num1[i] - '0';
		int factor;
		for (factor = 9; factor * num2 > dividend; --factor)
			;
		dividend -= factor * num2;
		quotient.push_back(factor + '0');
	}
	num1.number = quotient;
	num1.remove_leading_zeroes();
	return num1;
}

big_number operator/(const big_number &num1, const big_number &num2)
{
	big_number res(num1.number);
	res /= num2;
	return res;
}

istream &operator>>(istream &in, big_number &num)
{
	string buffer;
	in >> buffer;
	for (char digit : buffer)
	{
		if (!isdigit(digit))
			throw("ERROR :: Not a number");
		if (digit == '0' && num.number == "")
			continue;
		num.number.push_back(digit);
	}
	if (num.number == "")
		num.number = "0";
	return in;
}

ostream &operator<<(ostream &out, const big_number &num)
{
	cout << num.number;
	return cout;
}

big_number &power(big_number &num1, int exponent)
{
	big_number base(num1);
	big_number *res = new big_number(1);
	while (exponent)
	{
		if (exponent & 1)
			*res *= base;
		base *= base;
		exponent /= 2;
	}
	return *res;
}

big_number &sqrt(big_number &num1)
{
	if (num1 == 0 || num1 == 1)
	{
		big_number *res = new big_number(num1);
		return *res;
	}
	// binary search for finding sqrt
	big_number low, high, mid, square;
	low = 1;
	high = num1 / 2;
	big_number *res = new big_number(1);
	while (low <= high)
	{
		mid = (low + high) / 2;
		square = mid * mid;
		if (square == num1)
		{
			*res = mid;
			return *res;
		}
		else if (square < num1)
		{
			low = mid + 1;
			*res = mid;
		}
		else
			high = mid - 1;
	}
	return *res;
}

big_number &multiply_modulus(big_number &num1, big_number &num2, big_number &mod)
{
	big_number *res = new big_number(0), tmp1, tmp2;
	tmp1 = num1;
	tmp2 = num2;
	while (!tmp1.is_null())
	{
		if (tmp1.last_digit() & 1)
		{
			*res = (*res + tmp2) % mod;
		}
		tmp1.right_shift(1);
		tmp2.left_shift(1);
		tmp2 %= mod;
	}
	return *res;
}

big_number &power_modulus(big_number &num1, big_number &exponent, big_number &mod)
{
	big_number base;
	base = num1 % mod;
	if (exponent == 0)
		return *(new big_number(1));
	big_number tmp, *dump = new big_number();
	big_number trash;
	trash = exponent / 2;
	tmp = power_modulus(base, trash, mod);
	*dump = multiply_modulus(tmp, tmp, mod);
	// odd exponent
	if (exponent.last_digit() & 1)
	{
		*dump = multiply_modulus(base, *dump, mod);
		*dump %= mod;
		return *dump;
	}
	else
	{
		return *dump;
	}
}

big_number &get_random_number(int bit_size)
{
	random_device seed;
	mt19937 engine(seed());
	uniform_int_distribution<int> distribution(0, 1);
	big_number *num = new big_number(1); // to make the number odd
	for (int i = 1; i < bit_size - 1; ++i)
	{
		if (distribution(engine))
		{
			big_number tmp(2);
			tmp = power(tmp, i);
			*num += tmp;
		}
	}
	big_number tmp(2);
	tmp = power(tmp, bit_size - 1);
	*num += tmp; // to make the most significant bit high
	return *num;
}

big_number &gcd(big_number &num1, big_number &num2)
{
	big_number tmp1, *tmp2 = new big_number();
	tmp1 = num1;
	*tmp2 = num2;
	while (true) {
        big_number tmp;
		tmp = tmp1 % *tmp2;
        if (tmp.is_null())
            return *tmp2;
        tmp1 = *tmp2;
        *tmp2 = tmp;
    }
}

big_number &min(big_number &num1, big_number &num2)
{
	return (num1 < num2)? num1: num2;
}

big_number &max(big_number &num1, big_number &num2)
{
	return (num1 > num2)? num1: num2;
}

big_number &operator%=(big_number &num1, const big_number &num2)
{
	if (num2.is_null())
		throw("ERROR :: Zero division error");
	if (num1 < num2)
	{
		return num1;
	}
	if (num1 == num2)
	{
		num1.number = "0";
		return num1;
	}
	int i, n1 = num1.size();
	big_number dividend;
	for (i = 0; i < n1 && dividend * 10 + num1[i] - '0' < num2; ++i)
	{
		dividend = dividend * 10 + num1[i] - '0';
	}
	for (; i < n1; ++i)
	{
		dividend = dividend * 10 + num1[i] - '0';
		int factor;
		for (factor = 9; factor * num2 > dividend; --factor)
			;
		dividend -= factor * num2;
	}
	// at the end left over dividend is the remainder
	num1 = dividend;
	num1.remove_leading_zeroes();
	return num1;
}

big_number operator%(const big_number &num1, const big_number &num2)
{
	big_number res(num1.number);
	res %= num2;
	return res;
}

#endif // BIG_NUMBER_H
