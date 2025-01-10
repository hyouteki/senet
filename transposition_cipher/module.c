#define HASH_LEN 20
#define MAX_KEY_LEN 9

// imported APIs
unsigned int cstrlen(char *input);

// exported APIs
unsigned long hash(char *input);
int hash_len();

// djb2 hash
// REFERENCE: http://www.cse.yorku.ca/~oz/hash.html#djb2
unsigned long hash(char *input) {
	unsigned long hash = 5381;
	while (*input != 0) {
		hash = hash*33 + *input;
		input++;
	}
	return hash;
}

int hash_len() {
	return HASH_LEN;
}
