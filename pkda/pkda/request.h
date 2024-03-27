#ifndef PKDA_REQUESTS_H_
#define PKDA_REQUESTS_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#define MAX_REQUEST_LEN 10000

char *create_request_public_key_message(char *, char *, char *);

char *create_request_public_key_message(char *id_initiator,
										char *id_requested, char *nonce) {
	char *request = (char *) malloc (sizeof(char) * MAX_REQUEST_LEN);
	assert(request && "Error: cannot allocate memory");
	sprintf(request, "{\"service\":\"request-public-key\","
			"\"id-initiator\":\"%s\",\"id-requested\":\"%s\","
			"\"time\":%lu,\"nonce\":\"%s\"}", id_initiator,
			id_requested, (unsigned long) time(NULL), nonce);
	return request;
}

#endif // PKDA_REQUESTS_H_
