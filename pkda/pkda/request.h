#ifndef PKDA_REQUEST_H_
#define PKDA_REQUEST_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <gmp.h>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <jansson.h>
#include "rsa.h"

#define SERVER_PORT 6666
#define MAX_BUFFER_SIZE 10000
#define PKDA_E "65537"
#define PKDA_N "662301898900086280563369235571503136063408961425424293073766271072781807911678567160469252002508440143033677125709166319850891202267909129323826679019242540749194345034060529226377321735363290110199045801178998513803893245381318099967591575898939445190291940949507376209765831367454894205894045180121495499261365443592353041143477594699444475913508662053377142533418413333418718088874085795965156331233623965528933874295463858380678888109306494764712337200096926174855576311330169743585133175566774877252591191731066635041723346935277660875617341933673432178798773584458993865292096053045824574821342933398760069773566992849729936703021425032724505258874534588255854732984011088966682000696166656261128352109646804570768175716261804095940503220172010985248168359409730123644594297470268721009009570923252673603423160225737807263165170523164878458713262213757728917880939881170181755268413347133435655415491539621965171531718197592141946475449176562814265428561765008381698230810347873879846931572180937064757014711991322407076881505615362132288518673400936122704321154482695758374681480937489070854176890104436257311623381762185073814627159219122440331505399890649750450765175765350404092862241528856083003711741300960236860359247271"

static char *create_request_public_key_message(char *, char *, char *);
char *encrypt_message(char *, char *, char *);
char *decrypt_message(char *, char *, char *);
char *encrypt_request(char *);
char *decrypt_response(char *);
void request_public_key(char *, char *, char *, char **, char **, int);

static char *create_request_public_key_message(char *id_initiator,
										char *id_requested, char *nonce) {
	char *request = (char *) malloc (sizeof(char) * MAX_BUFFER_SIZE);
	assert(request && "Error: cannot allocate memory");
	sprintf(request, "{\n    \"service\": \"request-public-key\",\n"
			"    \"id-initiator\": \"%s\",\n    \"id-requested\": \"%s\",\n"
			"    \"time\": %lu,\n    \"nonce\": \"%s\"\n}", id_initiator,
			id_requested, (unsigned long) time(NULL), nonce);
	return request;
}

char *encrypt_message(char *message, char *e_str, char *n_str) {	
	mpz_t e, n;
	mpz_inits(e, n, NULL);
	mpz_set_str(e, e_str, 10);
	mpz_set_str(n, n_str, 10);
	char *encrypted_message = encrypt(message, e, n);
	mpz_clears(e, n, NULL);
	return encrypted_message;
}

char *decrypt_message(char *encrypted_message, char *d_str, char *n_str) {
	mpz_t d, n;
	mpz_inits(d, n, NULL);
	mpz_set_str(d, d_str, 10);
	mpz_set_str(n, n_str, 10);
	char *message = decrypt(encrypted_message, d, n);
	mpz_clears(d, n, NULL);
	return message;
}

char *encrypt_request(char *request) {
	return encrypt_message(request, PKDA_E, PKDA_N);
}

char *decrypt_response(char *encrypted_response) {
	return decrypt_message(encrypted_response, PKDA_E, PKDA_N);
}

void request_public_key(char * id_initiator, char *id_requested, char *nonce_sent,
						char **e, char **n, int debug) {
	int client_sock = socket(AF_INET, SOCK_STREAM, 0);
	assert(client_sock != 0 && "Error: could not create socket");

	struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT); 
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); 

	assert(connect(client_sock, (struct sockaddr *)&server_addr,
				   sizeof(server_addr)) >= 0 && "Error: connection failed");

	char *request = create_request_public_key_message(id_initiator, id_requested, nonce_sent);
	if (debug) printf("Info: REQUEST START\n%s\nInfo: REQUEST END\n", request);
	char *encrypted_request = encrypt_request(request);
	assert(send(client_sock, encrypted_request, slen(encrypted_request), 0)
		   == slen(encrypted_request) && "Error: send failed");
	if (debug) printf("Info: request sent\n");
	
	unsigned int bytes_received;
	char buffer[MAX_BUFFER_SIZE] = {0};
	while ((bytes_received = recv(client_sock, buffer, MAX_BUFFER_SIZE, 0)) > 0) {
		if (debug) printf("Info: response received\n");
		buffer[bytes_received] = 0;
		char *response = decrypt_response(buffer);
		json_error_t error;
		json_t *json_obj = json_loads(response, 0, &error);
		true_unless_kill(json_obj != NULL, "failed to parse json");
		true_unless_kill(json_is_object(json_obj) ,
						 "received response is not a json object");
				
		if (debug) {
			printf("Info: RESPONSE START\n");
			print_json_object(json_obj);
			printf("Info: RESPONSE END\n");
		}

		char *nonce_recv = json_get_string(json_obj, "nonce");
		true_unless_kill(scmp(nonce_sent, nonce_recv), "nonce verification failed");
		
		*e = json_get_string(json_obj, "e");
		*n = json_get_string(json_obj, "n");
		break;
	}
    close(client_sock);
}

#endif // PKDA_REQUEST_H_
