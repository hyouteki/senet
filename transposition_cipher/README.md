## Transposition cipher
- Encryption & decryption using transposition. 
- Developing the software to launch a brute-force attack to discover the key. 
> Here assume that the key length is known to be 9 or less.

## Assumptions
- Max plaintext and ciphertext length 256 (Hardcoded can be increased).
- Character set: All lowercase english alphabets except for 'z' which is reserved for padding.

## Guide
- __Encryption__
  ``` console
  ./transposition_cipher encrypt "<plaintext>" <key_len> <space-seperated-key>
  ./transposition_cipher encrypt "helloworld" 7 2 3 1 4 6 5 7
  
  Plaintext: helloworldaicedbeehdhfihgcdejd
  Ciphertext: ldehzhrdfjelbidlaegzwcddzoihczoehez
  ```
- __Decryption__
  ``` console
  ./transposition_cipher decrypt "<ciphertext>" <key_len> <space-seperated-key>
  ./transposition_cipher decrypt "ldehzhrdfjelbidlaegzwcddzoihczoehez" 7 2 3 1 4 6 5 7
  
  Plaintext: helloworld
  Hash: aicedbeehdhfihgcdejd
  ```
- __Hash__
  ``` console
  ./transposition_cipher hash "<plaintext>"
  ./transposition_cipher hash "helloworld"
  
  Hash: aicedbeehdhfihgcdejd
  ```
- __Brute force__
  ``` console
  ./transposition_cipher bruteforce "<ciphertext>"
  ./transposition_cipher bruteforce "ldehzhrdfjelbidlaegzwcddzoihczoehez"
  
  Key: 2 3 1 4 6 5 7
  Plaintext: helloworld
  Hash: aicedbeehdhfihgcdejd
  ```

## Quick start
- Install [FASM](https://flatassembler.net/download.php)
- Build the project using `make`

## References
- Hash function: http://www.cse.yorku.ca/~oz/hash.html
