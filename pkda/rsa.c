#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>
#include <assert.h>
#include "pkda/utils.h"
#include "pkda/rsa.h"

int main(int argc, char **argv) {
	assert(argc >= 2 && "Error: subcommand not present");
	if (scmp(argv[1], "genkeys")) {
		unsigned int bit_size = (argc == 3)? stoul(argv[2]): BIT_SIZE;
		
		mpz_t e, d, n;
		mpz_inits(e, d, n, NULL);

		genkeys(bit_size, e, d, n);

		/* write_key_to_file(e, n, "publickey.txt", 'e'); */
		/* write_key_to_file(d, n, "privatekey.txt", 'd'); */
		
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
	}
    return 0;
}
