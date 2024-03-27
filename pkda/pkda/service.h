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

User *users;

void insert_user(char *, mpz_t, mpz_t);

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
