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

User *users = NULL;

User *create_user(char *, mpz_t, mpz_t);
void kill_user(User *);
void insert_user(User *);
void print_user(User *);
void print_users();
char *json_get_string(json_t *, char *);
void add_users_from_file(char *);

User *create_user(char *id, mpz_t publickey, mpz_t n) {
	User *user = malloc(sizeof(User));
	true_unless_kill(user != NULL, "failed to allocate memory");
	user->id = (char *) malloc(sizeof(char)*(slen(id)+1));
	user->id = id;
	mpz_inits(user->publickey, user->n, NULL);
	mpz_set(user->publickey, publickey);
	mpz_set(user->n, n);
	return user;
}

void kill_user(User *user) {
	mpz_clears(user->publickey, user->n, NULL);
	free(user);
}

void insert_user(User *user) {
	User *obj = malloc(sizeof(User));
	true_unless_kill(obj != NULL, "failed to allocate memory");
	obj->id = user->id;
	mpz_inits(obj->publickey, obj->n, NULL);
	mpz_set(obj->publickey, user->publickey);
	mpz_set(obj->n, user->n);
	obj->next = users;
	users = obj;
	kill_user(user);
}

void print_user(User *user) {
	gmp_printf("id: %s\n e: %Zd\n n: %Zd\n", user->id, user->publickey, user->n);
}

void print_users() {
	User *user = users;
	while (user != NULL) {
		print_user(user);
		gmp_printf("\n");
		user = user->next;
	}
}

char *json_get_string(json_t *json_obj, char *field) {
	json_t *field_val = json_object_get(json_obj, field);
	true_unless_kill(field_val != NULL && json_is_string(field_val), "a field is incorrect");
	return (char *) json_string_value(field_val);
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
	
	json_error_t error;
	json_t *json_obj = json_loads(buffer, 0, &error);
	true_unless_kill(json_obj != NULL, "failed to parse json");
	true_unless_kill(json_is_object(json_obj) , "file is not a json object");
	
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
			mpz_set_str(publickey, json_get_string(value, "publickey"), 10);
			mpz_set_str(n, json_get_string(value, "n"), 10);
			User *user = create_user(id, publickey, n);
			insert_user(user);
        }
    }

	/* print_users(); */
	
	json_decref(json_obj);
}
