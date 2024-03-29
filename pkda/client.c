#include <stdio.h>
#include "pkda/request.h"

int main() {
	char *e, *n;
	request_public_key("0", "1", "the one who lays in dormant...", &e, &n, 1);
    return 0;
}
