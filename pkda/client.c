#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <assert.h>
#include <jansson.h>
#include "pkda/utils.h"
#include "pkda/request.h"

#define SERVER_PORT 6666
#define MAX_BUFFER_SIZE 10000

int main(int argc, char **argv)
{
	int client_sock = socket(AF_INET, SOCK_STREAM, 0);
	assert(client_sock != 0 && "Error: could not create socket");

	struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT); 
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); 

	assert(connect(client_sock, (struct sockaddr *)&server_addr,
				   sizeof(server_addr)) >= 0 && "Error: connection failed");

	char *nonce_sent = "the one who lays dormant...";
	char *request = create_request_public_key_message("0", "1", nonce_sent);
	printf("Info: REQUEST START\n%s\nInfo: REQUEST END\n", request);
	char *encrypted_request = encrypt_request(request);
	assert(send(client_sock, encrypted_request, slen(encrypted_request), 0)
		   == slen(encrypted_request) && "Error: send failed");
	printf("Info: request sent\n");
	
	unsigned int bytes_received;
	char buffer[MAX_BUFFER_SIZE] = {0};
	while ((bytes_received = recv(client_sock, buffer, MAX_BUFFER_SIZE, 0)) > 0) {
		printf("Info: response received\n");
		buffer[bytes_received] = 0;
		char *response = decrypt_response(buffer);
		json_error_t error;
		json_t *json_obj = json_loads(response, 0, &error);
		true_unless_kill(json_obj != NULL, "failed to parse json");
		true_unless_kill(json_is_object(json_obj) ,
						 "received response is not a json object");
				
		printf("Info: RESPONSE START\n");
		print_json_object(json_obj);
		printf("Info: RESPONSE END\n");

		char *nonce_recv = json_get_string(json_obj, "nonce");
		if (scmp(nonce_sent, nonce_recv))
			printf("Info: successfully verified response\n");
		else printf("Info: failed response verification\n");

	}
    close(client_sock);
    return 0;
}
