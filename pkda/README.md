## PKDA

### Request structure
``` json
{
	"service": "request-public-key",
	"id-initiater": "<id>",
	"id-responder": "<id>",
	"time": "<time>",
	"nonce": "<nonce>"
}
```

## [RSA](https://en.wikipedia.org/wiki/RSA_(cryptosystem)) (Rivest–Shamir–Adleman)

### Guide
- __Generate keys__: This will generate keys of specified key size (default-2048) and print the relevant informations to the console (p & q: choosen random prime numbers, phi, n, e: encryption key, and d: decryption key) also store the keys in text files (publickey.txt and privatekey.txt).
  ``` console
  ./rsa genkeys <key_size: optional; default-2048>
  ```
  
## Quick start
- Install the required packages using `make install`.
- Build the project using `make`.