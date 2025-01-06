#define HASH_LEN 20
#define MAX_KEY_LEN 9

extern void print(char* input);

// djb2 hash
// REFERENCE: http://www.cse.yorku.ca/~oz/hash.html#djb2
unsigned long hash(char *input) {
	unsigned long hash = 5381;
	print(input);
	while (*input != 0) {
		hash = hash*33 + *input;
		input++;
	}
	return hash;
}
