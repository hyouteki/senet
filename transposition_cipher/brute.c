#include <stdio.h>

extern char* decrypt(char*, int, int*, int);
extern char* hash(char*, int);

void decrypt_test() {
	char* ciphertext = "llHoerld -o-w-";
	int ciphertext_len = 15;
	int key[] = {2, 3, 1, 4, 6, 5, 7};
	int key_len = 7;
	char* plaintext = decrypt(ciphertext, ciphertext_len, key, key_len);
	printf("%s", plaintext);
}

void hash_test() {
	char* plaintext = "Hello world";
	int plaintext_len = 12;
	char* plaintext_hash = hash(plaintext, plaintext_len);
	printf("%s", plaintext_hash);
}

int main() {
	/* decrypt_test(); */
	/* hash_test(); */
	return 0;
}
