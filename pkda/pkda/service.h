#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <jansson.h>
#include "../utils.h"

typedef struct User {
	char *id;
	mpz_t publickey;
	mpz_t n;
	struct User* next;
} User;

User *users = NULL;

void insert_user(char *, mpz_t, mpz_t);
void add_users_from_file(char *);

void insert_user(char *id, mpz_t publickey, mpz_t n) {
	User *user = malloc(sizeof(User));
	true_unless_kill(user != NULL, "could not allocate memory");
	user->id = (char *) malloc(sizeof(char)*(slen(id)+1));
	user->id = id;
	mpz_inits(user->publickey, user->n, NULL);
	mpz_set(user->publickey, publickey);
	mpz_set(user->n, n);
	user->next = users;
	users = user;
}

void add_users_from_file(char *filename) {
	FILE *file = fopen(filename, "rb");
	true_unless_kill(file != NULL, "could not open file");

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char *)malloc(file_size + 1);
    if (!buffer) {
        perror("Error: failed to allocate memory\n");
        fclose(file);
    }

    size_t bytes_read = fread(buffer, 1, file_size, file);
    if (bytes_read != file_size) {
        perror("Error: failed to read entire file\n");
        fclose(file);
        free(buffer);
		return;
    }
    buffer[bytes_read] = '\0';
    fclose(file);

	/* printf("%s\n", buffer); */
}
