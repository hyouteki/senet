#ifndef PKDA_SERVICE_H_
#define PKDA_SERVICE_H_

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <jansson.h>
#include "utils.h"

typedef struct User {
	char *id;
	mpz_t publickey;
	mpz_t n;
	struct User* next;
} User;

void insert_user(User **, char *, mpz_t, mpz_t);
void print_user(User *);
void print_users(User **);
User *get_user(User **, char *);
json_t *read_json_file(char *);
void add_users_from_file(User **, char *);

void insert_user(User **users, char *id, mpz_t publickey, mpz_t n) {
	User *user = (User *) malloc(sizeof(User));
	true_unless_kill(user != NULL, "failed to allocate memory");
	user->id = (char *) malloc(sizeof(char)*(slen(id)+1));
	for (int i = 0; i < slen(id); ++i) user->id[i] = id[i];
	user->id[slen(id)] = 0;
	mpz_inits(user->publickey, user->n, NULL);
	mpz_set(user->publickey, publickey);
	mpz_set(user->n, n);
	user->next = *users;
	*users = user;
}

void print_user(User *user) {
	printf("id: %s\n", user->id);
	gmp_printf(" e: %Zd\n n: %Zd\n", user->publickey, user->n);
}

void print_users(User **users) {
	User *user = *users;
	while (user != NULL) {
		print_user(user);
		gmp_printf("\n");
		user = user->next;
	}
}

User *get_user(User **users, char *id) {
	User *user = *users;
	while (user != NULL) {
		if (scmp(id, user->id)) return user;
		user = user->next;
	}
	return NULL;
}

json_t *read_json_file(char *filename) {
	FILE *file = fopen(filename, "rb");
	true_unless_kill(file != NULL, "could not open file");

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char *)malloc(file_size + 1);
    if (!buffer) {
        perror("Error: failed to allocate memory\n");
        fclose(file);
	    exit(1);
    }

    size_t bytes_read = fread(buffer, 1, file_size, file);
    if (bytes_read != file_size) {
        perror("Error: failed to read entire file\n");
        fclose(file);
        free(buffer);
		exit(1);
    }
    buffer[bytes_read] = '\0';
    fclose(file);
	
	json_error_t error;
	json_t *json_obj = json_loads(buffer, 0, &error);
	true_unless_kill(json_obj != NULL, "failed to parse json");
	true_unless_kill(json_is_object(json_obj) , "file is not a json object");

	return json_obj;
}

void add_users_from_file(User **users, char *filename) {
	json_t *json_obj = read_json_file(filename);
	
	/* print_json_object(json_obj); */

	json_t *users_array = json_object_get(json_obj, "users");
    if (!json_is_array(users_array)) {
        perror("Error: missing key 'users'\n");
        json_decref(json_obj);
        return;
    }
	
    size_t index;
    json_t *value;
	
    json_array_foreach(users_array, index, value) {
        if (json_is_object(value)) {
			char *id = json_get_string(value, "id");
			mpz_t publickey, n;
			mpz_inits(publickey, n, NULL);
			mpz_set_str(publickey, json_get_string(value, "publickey"), 10);
			mpz_set_str(n, json_get_string(value, "n"), 10);
			insert_user(users, id, publickey, n);
			mpz_clears(publickey, n, NULL);
        }
    }

	/* print_users(users); */
	
	json_decref(json_obj);
}

#endif // PKDA_SERVICE_H_
