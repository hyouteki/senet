Implementation (in C and C++) of [GNU/GMP](https://en.wikipedia.org/wiki/GNU_Multiple_Precision_Arithmetic_Library) (GNU Multiple Precision Arithmetic Library)'s mpz_t; number representation in form of a string. Similar to python's int. Facilitating arithmetric operations on integers of any size. GNU/GMP also have representations for floats and rational numbers but this implementation does not support that. 

## big\_number
[Implementation](./big_number.h) of GNU/GMP's mpz\_t in C++; also supports generation of large [prime numbers](prime_number.h). <br>
Run the tests using `make big_number.test`

## Diffie-Hellman
[Diffie-Hellman](https://en.wikipedia.org/wiki/Diffie%E2%80%93Hellman_key_exchange) is a key exchange protocol allowing two parties to generate a shared secret over an insecure channel. They independently generate public-private key pairs, then exchange public keys to compute a shared secret, used for encryption. Implementation can be found [here](./diffie_hellman.cpp); made using big\_number.h and prime\_number.h custom cpp libraries. <br>
Run the tests using `make diffie_hellman`
``` console
a: 13850290419018498113
b: 10423192389116432677
g: 42043
n: 263703706722661637931335369148187364569
A: 130096087070979433531158641174751366482
B: 134249355232484277218449036949679888768
ka: 31583201520126415361142830650475280700
kb: 31583201520126415361142830650475280700
samekey: 1
```

## Large
[Implementation](./large.h) of GNU/GMP's mpz\_t in C++. <br>
Run the tests using `make large.test`
