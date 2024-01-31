#include <stdio.h>
#include <stdlib.h>

#define HASH_LEN 20
#define MAX_KEY_LEN 9

extern char* decrypt(char*, int, int*, int);
extern unsigned long hash(char*);
extern char* encrypt(char*, int, int*, int);

unsigned long stoul(char*);
int slen(char*);
int scmp(char*, char*);
char* sconcat(char*, char*);
void swap(int*, int*);
int test(char*, int*, int);
void brute(char*);
int* resetkey(int*, int);
void printkey(int*, int);
void process_decryptedtext(char*, char**, char**);
char* transform_hash(unsigned long);
unsigned long retransform_hash(char*);

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

char* sconcat(char* a, char* b) {
	char* x = (char*) malloc(slen(a)+slen(b)-1);
	char* res = x;
	char* ptr = a;
	while (*ptr != 0) {
		*res = *ptr;
		++res;
		++ptr;
	}
	ptr = b;
	while (*ptr != 0) {
		*res = *ptr;
		++res;
		++ptr;
	}
	*res = 0;
	return x;
}

void swap(int* a, int* b) {
	int t = *a;
	*a = *b;
	*b = t;
}

int test(char* ciphertext, int* key, int key_len) {
	char* decrypted_text = decrypt(ciphertext, slen(ciphertext), key, key_len);
	char* plaintext, *hash_val;
	process_decryptedtext(decrypted_text, &plaintext, &hash_val);
	int ans = hash(plaintext) == retransform_hash(hash_val);
	free(plaintext);
	free(hash_val);
	return ans;
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

void brute_helper(char* ciphertext, int* key, int size, int n) {
	if (size == 1) {
		if (test(ciphertext, key, n)) {
			printf("Key: ");
			printkey(key, n);
			char* decrypted_text = decrypt(ciphertext, slen(ciphertext), key, n);
			char* plaintext, *hash_val;
			process_decryptedtext(decrypted_text, &plaintext, &hash_val);
			printf("Plaintext: %s\n", plaintext);
			printf("Hash: %s\n", hash_val);
			free(plaintext);
			free(hash_val);			
			exit(0);
		}
		return;
	}
	for (int i = 0; i < size; ++i) {
		brute_helper(ciphertext, key, size-1, n);
		if (size & 1) swap(&key[0], &key[size-1]);
		else swap(&key[i], &key[size-1]);
	}
}

void brute(char* ciphertext) {
	int ciphertext_strlen = slen(ciphertext)-1;
	int key[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	for (int key_len = 1; key_len <= MAX_KEY_LEN; ++key_len) {
		if (ciphertext_strlen % key_len != 0) continue;
		resetkey(key, key_len);
		brute_helper(ciphertext, key, key_len, key_len);
	}
}

void process_decryptedtext(char* text, char** ptr_plaintext, char** ptr_hash) {
	char* plaintext_val = (char*) malloc(slen(text)-HASH_LEN);
	char* hash_val = (char*) malloc(HASH_LEN+1);
	int len = slen(text);
	for (int i = 0; i < len-HASH_LEN-1; ++i) {
		*(plaintext_val+i) = *(text+i);
	}
	*(plaintext_val+slen(text)-HASH_LEN-1) = 0;
	for (int i = 0; i < HASH_LEN; ++i) {
		*(hash_val+i) = *(text+len-HASH_LEN-1+i);
	}
	*(hash_val+HASH_LEN) = 0;
	*ptr_plaintext = plaintext_val;
	*ptr_hash = hash_val;
}

char* transform_hash(unsigned long hash_val) {
	char* res = (char*) malloc(HASH_LEN+1);
	*(res+20) = 0;
	for (int i = 19; i >= 0; --i) {
		*(res+i) = hash_val%10 + 'a';
		hash_val /= 10;
	}
	return res;
}

unsigned long retransform_hash(char* hash_val) {
	unsigned long x = 0;
	char* ptr = hash_val;
	while (*ptr != 0) {
		x = x*10 + *ptr - 'a';
		++ptr;
	}
	return x;
}

int main(int argc, char** argv) {
	if (scmp(argv[1], "encrypt")) {
		int key_len = (int) stoul(argv[3]);
		int key[key_len];
		for (int i = 0; i < key_len; ++i) {
			key[i] = (int) stoul(argv[i+4]);
		}
		printf("Key: ");
		printkey(key, key_len);
		char* transformed_hash = transform_hash(hash(argv[2]));
		char* plaintext = sconcat(argv[2], transformed_hash);
		printf("Plaintext: %s\n", plaintext);
		printf("Ciphertext: %s\n", encrypt(plaintext, slen(plaintext), key, key_len));
		free(transformed_hash);
		free(plaintext);
	} else if (scmp(argv[1], "decrypt")) {
		char* ciphertext = argv[2];
		int key_len = (int) stoul(argv[3]);
		int key[key_len];
		for (int i = 0; i < key_len; ++i) {
			key[i] = (int) stoul(argv[i+4]);
		}
		printf("Key: ");
		printkey(key, key_len);
		char* decrypted_text = decrypt(ciphertext, slen(ciphertext), key, key_len);
		char* plaintext, *hash_val;
		process_decryptedtext(decrypted_text, &plaintext, &hash_val);
		printf("Plaintext: %s\n", plaintext);
		printf("Hash: %s\n", hash_val);
		free(plaintext);
		free(hash_val);
	} else if (scmp(argv[1], "hash")) {
		printf("Hash: %s\n", transform_hash(hash(argv[2])));
	} else if (scmp(argv[1], "bruteforce")) {
		char* ciphertext = argv[2];
		printf("Ciphertext: %s\n", ciphertext);
		brute(ciphertext);
	} else {
		printf("error: invalid subcommand\n");
		exit(1);
	}
 	return 0;
}
