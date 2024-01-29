#include <stdio.h>

extern char* decrypt(char*, int, int*, int);
extern char* hash(char*, int);
extern int bye(int);

unsigned long stoul(char*);
int slen(char*);
void swap(int*, int*);
int test(char*, unsigned long, int*, int);
void brute(char*, char*);
int* resetkey(int*, int);

unsigned long stoul(char* hash_val) {
	unsigned long x = 0;
	char* ptr = hash_val;
	while (*ptr != 0) {
		x = 10*x + *ptr - 48;
		++ptr;
	}
	return x;
}

int slen(char* text) {
	int len = 0;
	char* ptr = text;
	while (*ptr != 0) {
		++len;
		++ptr;
	}
	return len+1;
}

void swap(int* a, int* b) {
	int t = *a;
	*a = *b;
	*b = t;
}

int test(char* ciphertext, unsigned long hash_val, int* key, int key_len) {
	char* plaintext = decrypt(ciphertext, slen(ciphertext), key, key_len);
	char* plaintext_hash = hash(plaintext, slen(plaintext));
	return stoul(plaintext_hash) == hash_val;
}

void print_key(int* key, int len) {
	for (int i = 0, *ptr = key; i < len; ++i, ++ptr) {
		printf("%d ", *ptr);
	}
	printf("\n");
}

int* resetkey(int* key, int key_len) {
	for (int i = 0; i < key_len; ++i) {
		key[i] = i+1;
	}
}

void brute_helper(char* ciphertext, unsigned long hash_val, int* key, int size, int n) {
	if (size == 1) {
		if (test(ciphertext, hash_val, key, n)) {
			printf("Key: ");
			print_key(key, n);
			printf("Decrypted text: %s\n", decrypt(ciphertext, slen(ciphertext), key, n));
			bye(0);
		}
		return;
	}
	for (int i = 0; i < size; ++i) {
		brute_helper(ciphertext, hash_val, key, size-1, n);
		if (size & 1) swap(&key[0], &key[size-1]);
		else swap(&key[i], &key[size-1]);
	}
}

void brute(char* ciphertext, char* hash_val) {
	unsigned long plaintext_hash = stoul(hash_val);
	int ciphertext_strlen = slen(ciphertext)-1;
	int key[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	for (int key_len = 1; key_len <= 9; ++key_len) {
		if (ciphertext_strlen % key_len != 0) continue;
		resetkey(key, key_len);
		brute_helper(ciphertext, plaintext_hash, key, key_len, key_len);
	}
}

int main() {
	brute("llHoerld -o-w-", hash("Hello world", 12));
	return 0;
}
