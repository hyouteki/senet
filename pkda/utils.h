#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>

int scmp(char *, char *);
unsigned int stoul(char *);
unsigned int slen(const char *);
void true_unless_kill(int, char *);

int scmp(char *a, char *b) {
	char *ptr_a = a, *ptr_b = b;
	while (*ptr_a && *ptr_b) {
		if (*ptr_a != *ptr_b) return 0;
		++ptr_a;
		++ptr_b;
	}
	return *ptr_a == *ptr_b;
}

unsigned int stoul(char *a) {
	char *ptr = a;
	unsigned int num = 0;
	while (*ptr) {
		num = num*10 + *ptr - '0';
		++ptr;
	}
	return num;
}

unsigned int slen(const char *a) {
	const char *ptr = a;
	unsigned int len = 0;
	while (*ptr != 0) {
		++len;
		++ptr;
	}
	return len;
}

void true_unless_kill(int condition, char *message) {
	if (!condition) {
		fprintf(stderr, "Error: %s\n", message);
		exit(1);
	}
}

#endif // UTILS_H_
