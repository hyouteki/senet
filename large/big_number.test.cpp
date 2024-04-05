#include "big_number.h"
using namespace std;

int main(int argc, char **argv) {
	int n1, n2, n3;
	if (argc == 1) cin >> n1 >> n2 >> n3;
	if (argc == 4) {
		n1 = atoi(argv[1]);
		n2 = atoi(argv[2]);
		n3 = atoi(argv[3]);
	}
	big_number num1(n1), num2(n2), num3(n3);
	cout << "num1: " << num1 << endl;
	cout << "num2: " << num2 << endl;
	cout << "num3: " << num3 << endl;
	cout << "pow: " << power(num1, n2) << endl;
	cout << "num1: " << num1 << endl;
	cout << "num2: " << num2 << endl;
	cout << "num3: " << num3 << endl;
	cout << "sqrt num1: " << sqrt(num1) << endl;
	cout << "num1: " << num1 << endl;
	cout << "num2: " << num2 << endl;
	cout << "num3: " << num3 << endl;
	cout << "mulmod: " << multiply_modulus(num1, num2, num3) << endl;
	cout << "num1: " << num1 << endl;
	cout << "num2: " << num2 << endl;
	cout << "num3: " << num3 << endl;
	cout << "powmod: " << power_modulus(num1, num2, num3) << endl;
	cout << "num1: " << num1 << endl;
	cout << "num2: " << num2 << endl;
	cout << "num3: " << num3 << endl;
	cout << "gcd: " << gcd(num1, num2) << endl;
	cout << "num1: " << num1 << endl;
	cout << "num2: " << num2 << endl;
	cout << "num3: " << num3 << endl;
	cout << "min: " << min(num1, num2) << endl;
	cout << "num1: " << num1 << endl;
	cout << "num2: " << num2 << endl;
	cout << "num3: " << num3 << endl;
	return 0;
}
