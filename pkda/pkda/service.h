#include <stdio.h>
#include <stdlib.h>
#include <jansson.h>
#include "utils.h"

typedef struct User {
	char *id;
	char *publickey;
	char *n;
	struct User* next;
} User;

User *users = NULL;

void kill_user(User *);
void insert_user(char *, char *, char *);
void print_user(User *);
void print_users();
char *json_get_string(json_t *, char *);
void add_users_from_file(char *);

void kill_user(User *user) {
	free(user);
}

void insert_user(char *id, char *publickey, char *n) {
	User *user = malloc(sizeof(User));
	true_unless_kill(user != NULL, "could not allocate memory");
	user->id = id;
	user->publickey = publickey;
	user->n = n;
	user->next = users;
	users = user;
}

void print_user(User *user) {
	printf("id: %s\n e: %s\n n: %s\n", user->id, user->publickey, user->n);
}

void print_users() {
	User *user = users;
	while (user != NULL) {
		print_user(user);
		printf("\n");
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
			insert_user((char *) json_get_string(value, "id"),
						(char *) json_get_string(value, "publickey"),
						(char *) json_get_string(value, "n"));
        }
    }
	json_decref(json_obj);
}
