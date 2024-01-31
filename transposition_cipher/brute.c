#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_LEN 20
#define MAX_KEY_LEN 9

extern char* decrypt(char*, int, int*, int);
extern unsigned long hash(char*);
extern int bye(int);

unsigned long stoul(char*);
int slen(char*);
int scmp(char*, char*);
void swap(int*, int*);
int test(char*, unsigned long, int*, int);
void brute(char*, unsigned long);
int* resetkey(int*, int);
void printkey(int*, int);
unsigned long process_ciphertext(char** text);

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

int scmp(char* a, char* b) {
	char* ptr_a = a, *ptr_b = b;
	while (*ptr_a != 0 && *ptr_b != 0) {
		if (*ptr_a != *ptr_b) return 0;
		++ptr_a;
		++ptr_b;
	}
	return *ptr_a == *ptr_b;
}

void swap(int* a, int* b) {
	int t = *a;
	*a = *b;
	*b = t;
}

int test(char* ciphertext, unsigned long hash_val, int* key, int key_len) {
	char* plaintext = decrypt(ciphertext, slen(ciphertext), key, key_len);
	unsigned long plaintext_hash = hash(plaintext);
	return plaintext_hash == hash_val;
}

void printkey(int* key, int len) {
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
			printkey(key, n);
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

void brute(char* ciphertext, unsigned long hash_val) {
	int ciphertext_strlen = slen(ciphertext)-1;
	int key[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	for (int key_len = 1; key_len <= MAX_KEY_LEN; ++key_len) {
		if (ciphertext_strlen % key_len != 0) continue;
		resetkey(key, key_len);
		brute_helper(ciphertext, hash_val, key, key_len, key_len);
	}
}

unsigned long process_ciphertext(char** text) {
	int text_len = slen(*text);
	unsigned long hash_val = stoul(*text+text_len-HASH_LEN-1);
	char* ciphertext = (char*) malloc(text_len-HASH_LEN), *ptr = *text;
	for (int i = 0; i < text_len-HASH_LEN-1; ++i) {
		*(ciphertext+i) = *(ptr+i);
	}
	*(ciphertext+text_len-HASH_LEN-1) = 0;
	*text = ciphertext;
	return hash_val;
}

int main(int argc, char** argv) {
	if (scmp(argv[1], "encrypt")) {
		printf("encrypt\n");
	} else if (scmp(argv[1], "hash")) {
		printf("%lu\n", hash(argv[2]));
	} else if (scmp(argv[1], "brute")) {
		char* text = argv[2];
		unsigned long hash_val = process_ciphertext(&text);
		printf("Ciphertext: %s\n", text);
		printf("Hash: %lu\n", hash_val);
		brute(text, hash_val);
		free(text);
	}
	return 0;
}
