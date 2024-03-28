#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <assert.h>
#include <jansson.h>
#include "pkda/utils.h"
#include "pkda/request.h"

#define SERVER_PORT 6666

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

	char *request = create_request_public_key_message("0", "1", "nonce");
	char *encrypted_request = encrypt_request(request);
	assert(send(client_sock, encrypted_request, slen(encrypted_request), 0)
		   == slen(encrypted_request) && "Error: send failed");
	
    close(client_sock);
    return 0;
}
