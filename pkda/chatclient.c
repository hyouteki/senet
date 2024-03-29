#include <stdio.h>
#include "pkda/service.h"
#include "pkda/request.h"
#include "pkda/utils.h"

#define PKDA_PORT 6666

int main(int argc, char **argv) {
	true_unless_kill(argc == 5, "Usage: ./chatclient <my-key.json>"
					 " <other-id> <my-port> <other-port>\n");
	
	unsigned int my_port = stoul(argv[3]), other_port = stoul(argv[4]);
	true_unless_kill(my_port != PKDA_PORT, "Error: my port is "
					 "same as pkda port");
	true_unless_kill(other_port != PKDA_PORT, "Error: other port is "
					 "same as pkda port");
	true_unless_kill(my_port != other_port, "Error: my port is "
					 "same as other port");
	
	json_t *json_obj = read_json_file(argv[1]);
	char *id = json_get_string(json_obj, "id");
	char *e = json_get_string(json_obj, "e");
	char *d = json_get_string(json_obj, "d");
	char *n = json_get_string(json_obj, "n");

	char *other_e, *other_n;
	request_public_key(id, argv[2], "what a wonderful world",
					   &other_e, &other_n, 0);

	int my_sock = socket(AF_INET, SOCK_STREAM, 0);
    true_unless_kill(my_sock != -1, "could not create socket");
	
	struct sockaddr_in other_addr;
    other_addr.sin_family = AF_INET;
    other_addr.sin_port = htons(other_port); 
    other_addr.sin_addr.s_addr = htonl(INADDR_ANY); 	
	
	true_unless_kill(connect(my_sock, (struct sockaddr*)&other_addr, sizeof(other_addr)) != -1,
					 "failed to connect");
	
	while (1) {
		char buffer[MAX_BUFFER_SIZE] = {0};
		printf("%s@%u > ", id, my_port);
        fgets(buffer, MAX_BUFFER_SIZE, stdin);
		char *encrypted_message = encrypt_message(buffer, other_e, other_n);
		send(my_sock, encrypted_message, slen(encrypted_message), 0);
		
		unsigned int bytes_received = recv(my_sock, buffer, MAX_BUFFER_SIZE, 0);
		true_unless_kill(bytes_received > 0, "failed to receive message");
		buffer[bytes_received] = 0;
		char *message = decrypt_message(buffer, d, n);
		printf("%s@%u > %s", argv[2], other_port, message);
	}
    close(my_sock);
    return 0;
}
