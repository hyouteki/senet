#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>
#include <assert.h>
#include "pkda/utils.h"
#include "pkda/rsa.h"

#define MAX_KEY_FILE_LEN 10000

int main(int argc, char **argv) {
	assert(argc >= 2 && "Error: subcommand not present");
	if (scmp(argv[1], "genkeys")) {
		unsigned int bit_size = (argc == 3)? stoul(argv[2]): BIT_SIZE;
		
		mpz_t e, d, n;
		mpz_inits(e, d, n, NULL);

		genkeys(bit_size, e, d, n);

		FILE *file = fopen("key.json", "w");
		assert(file != NULL && "Error: could not open file");

		char *e_str = mpz_get_str(NULL, 10, e);
		char *d_str = mpz_get_str(NULL, 10, d);
		char *n_str = mpz_get_str(NULL, 10, n);
		if (!e_str || !d_str || !n_str) {
			perror("Error: could not convert mpz to string");
			fclose(file);
			exit(1);
		}

		fprintf(file, "{\n    \"e\": \"%s\",\n    \"d\": \"%s\",\n"
				"    \"n\": \"%s\"\n}", e_str, d_str, n_str);
		free(file);
		
		gmp_printf("e: %Zd\n\n", e);
		gmp_printf("d: %Zd\n\n", d);
		gmp_printf("n: %Zd\n", n);
		
		mpz_clears(e, d, n, NULL);
	} else if (scmp(argv[1], "encrypt")) {
		true_unless_kill(argc == 5, "invalid commands");
		mpz_t e, n;
		mpz_inits(e, n, NULL);
		mpz_set_str(e, argv[3], 10);
		mpz_set_str(n, argv[4], 10);
		char *ciphertext = encrypt(argv[2], e, n);
		printf("%s\n", ciphertext);
		mpz_clears(e, n, NULL);
	} else if (scmp(argv[1], "decrypt")) {
		true_unless_kill(argc == 5, "invalid commands");
		mpz_t d, n;
		mpz_inits(d, n, NULL);
		mpz_set_str(d, argv[3], 10);
		mpz_set_str(n, argv[4], 10);
		char *plaintext = decrypt(argv[2], d, n);
		printf("%s\n", plaintext);
		mpz_clears(d, n, NULL);
	}
    return 0;
}
