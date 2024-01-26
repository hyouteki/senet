## Transposition cipher
- Encryption & decryption using transposition. 
- Developing the software to launch a brute-force attack to discover the key. 
> Here assume that the key length is known to be 9 or less.

## Assumptions
- Max cipher text length 256 (Hardcoded can be increased).
- Character set: Every ascii character except for '-' which is reserved for padding.

## Quick start
- Install [FASM](https://flatassembler.net/download.php)
``` console
make main
./main
```
