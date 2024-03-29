#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <jansson.h>
#include <gmp.h>
#include <assert.h>
#include "pkda/rsa.h"
#include "pkda/utils.h"
#include "pkda/response.h"
#include "pkda/service.h"

#define SERVER_PORT 6666
#define MAX_CONCURRENT_CONNS 5
#define MAX_BUFFER_SIZE 50000

User *users = NULL;

static void handle_request_public_key(json_t *, int);

static void handle_request_public_key(json_t *json_obj, int client_sock) {
    char *id_initiator = json_get_string(json_obj, "id-initiator");
    char *id_requested = json_get_string(json_obj, "id-requested");
    char *nonce = json_get_string(json_obj, "nonce");
	User *user_requested = get_user(&users, id_requested);

	char *e_str = mpz_get_str(NULL, 10, user_requested->publickey);
	char *n_str = mpz_get_str(NULL, 10, user_requested->n);
	if (!e_str || !n_str) {
		perror("Error: could not convert mpz to string");
		exit(1);
	}
	
	char response[MAX_BUFFER_SIZE];
	sprintf(response, "{\n    \"service\": \"request-public-key\",\n"
			"    \"id-requested\": \"%s\",\n    \"e\": \"%s\",\n"
			"    \"n\": \"%s\",\n    \"nonce\":\"%s\"\n}", id_requested,
			e_str, n_str, nonce);
	printf("Info: RESPONSE START\n%s\nInfo: RESPONSE END\n", response);
	char *encrypted_response = encrypt_response(response);
	
	send(client_sock, encrypted_response, slen(encrypted_response)+1, 0);
	printf("Info: response sent\n");
}

int main(int argc, char **argv) {
	add_users_from_file(&users, "./user_dataset.json");
	
	int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    true_unless_kill(server_sock != -1, "could not create socket");
	    
    int sock_opt = 1;
    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
			   (char*)&sock_opt, sizeof(sock_opt));

	struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT); 
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); 

	true_unless_kill(bind(server_sock, (struct sockaddr*)&server_addr,
						  sizeof(server_addr)) != -1, "could not bind server");
    
    true_unless_kill(listen(server_sock, MAX_CONCURRENT_CONNS) != -1,
					 "cannot listen to server");
	
	printf("Info: socket opened successfully\n");
	socklen_t addr_size = sizeof(server_addr);
	
	while (1) {
        int client_sock = accept(server_sock, (struct sockaddr*)&server_addr, &addr_size);
        if (client_sock == -1) continue;
		printf("Info: connected to client\n");
        
        int pid = fork();
        if (pid == 0) {
            char buffer[MAX_BUFFER_SIZE];
            int bytes_received;
            while ((bytes_received = recv(client_sock, buffer, sizeof(buffer), 0)) > 0) {
				char *request = decrypt_request(buffer);
				json_error_t error;
				json_t *json_obj = json_loads(request, 0, &error);
				true_unless_kill(json_obj != NULL, "failed to parse json");
				true_unless_kill(json_is_object(json_obj) ,
								 "received message is not a json object");
				
				printf("Info: REQUEST START\n");
				print_json_object(json_obj);
				printf("Info: REQUEST END\n");

				char *service = json_get_string(json_obj, "service");

				if (scmp(service, "request-public-key")) {
					handle_request_public_key(json_obj, client_sock);
				}

				json_decref(json_obj);
            } 
            close(client_sock);
        }
    }
    
    close(server_sock);
    return 0;
}
