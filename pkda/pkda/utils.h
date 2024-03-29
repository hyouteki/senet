#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <jansson.h>

int scmp(char *, char *);
unsigned int stoul(char *);
unsigned int slen(const char *);
char *sappend(char *, char *);
void true_unless_kill(int, char *);
void print_json_object(json_t *);
char *json_get_string(json_t *, char *);

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
	char *ptr = (char *) a;
	unsigned int len = 0;
	while (*ptr != 0) {
		++len;
		++ptr;
	}
	return len;
}

char *sappend(char *text, char *append_text) {
    char *rop = (char *)malloc(slen(text)+slen(append_text)+1);
	true_unless_kill(rop != NULL, "failed to allocate memory");
    unsigned int i = 0;
    while (text[i]) {
        rop[i] = text[i];
        ++i;
    }
    unsigned int j = 0;
    while (append_text[j]) {
        rop[i+j] = append_text[j];
        ++j;
    }
    rop[i+j] = 0;
    return rop;
}

void true_unless_kill(int condition, char *message) {
	if (!condition) {
		fprintf(stderr, "Error: %s\n", message);
		exit(1);
	}
}

void print_json_object(json_t *json_obj) {
	char *json_string = json_dumps(json_obj, JSON_INDENT(4));
	if (!json_string) {
		perror("Error: failed to stringify json object\n");
		return;
	}
	printf("%s\n", json_string);	
	free(json_string);
}

char *json_get_string(json_t *json_obj, char *field) {
	json_t *field_val = json_object_get(json_obj, field);
	true_unless_kill(field_val != NULL && json_is_string(field_val),
					 "a field is incorrect");
	return (char *) json_string_value(field_val);
}

#endif // UTILS_H_
