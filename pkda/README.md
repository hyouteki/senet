# [PKDA](https://en.wikipedia.org/wiki/Public_key_infrastructure) (Public Key Distribution Authority)
PKDA is a centralized authority which validates certificates and provides public keys for users on demand. This implementation of PKDA supports providing users with the public key of requested users and provides a technique to validate provided public keys.

## How it works
- PKDA first loads the [user_dataset.json](https://github.com/hyouteki/senet/blob/main/pkda/user_dataset.json) into the database (RAM) and waits for clients to send requests.
- Client prepares a JSON request of the following kind.
  ``` json
  {
  	"service": "request-public-key",
  	"id-initiater": "<id>",
  	"id-responder": "<id>",
  	"time": "<time>",
  	"nonce": "<nonce>"
  }
  ```
  Here, `id-initiator` refers to the ID for the user who is requesting this service, `id-requested` refers to the ID of the user whose public key is requested, `time` is current time in milliseconds from epoch for preventing reply attacks, and lastly, `nonce` is a generated string for validating the response from the PKDA. For example a valid request looks like so:
  ``` json
  {
  	"service": "request-public-key",
  	"id-initiator": "0",
  	"id-requested": "1",
  	"time": 1711713195,
  	"nonce": "the one who lays in dormant..."
  }
  ```
- Client encrypts the request with `PUBLIC_KEY` (e, n) of PKDA.
- PKDA decrypts the request with its `PRIVATE_KEY` (d, n), then proceeds to process and validate the request. After validation is done, it extracts the user's `PUBLIC_KEY` from the database and prepares a response of the following kind.
  ``` json
  {
  	"service": "request-public-key",
  	"id-requested": "<id-requested>",
  	"e": "<publickey>",
  	"n": "<modulus>",
  	"nonce": "<nonce>"
  }
  ```
  Again, here, `id-requested` refers to the ID of the user whose public key is requested, `e` & `n` are the public key of the requested user. Lastly, the `nonce` is the same as in the request, which the user can compare to verify that no tempering is happening on the response sent by the PKDA. For reference a sample response looks like so:
  ``` json
  {
  	"service": "request-public-key",
  	"id-requested": "1",
  	"e": "65537",
  	"n": "341353667926240456524536462...continued",
  	"nonce": "the one who lays in dormant..."
  }
  ```
- The response is encrypted with the `PRIVATE_KEY` of PKDA and sent to the user. The user then proceeds to process and validate the response by comparing the nonces. If the nonces are the same, that indicates the PKDA was adequately able to decrypt and process the request, and no tempering happened on the response, as it would have led to the change in the nonce received after decrypting the PKDA response.

> There is no need to encrypt the response via `PUBLIC_KEY` of the initiator, as that would result in confidentiality, which is unnecessary as all the information is public for anyone to access anyway.

## [RSA](https://en.wikipedia.org/wiki/RSA_(cryptosystem)) (Rivest–Shamir–Adleman)
> All the assymetric encrytion and decryption in PKDA are done via RSA.

As RSA requires the data to be in integer representation. Thus a custom encoding was needed to be developed. The preprocessing is as follows:
- First the string is chunked into multiple consecutive substrings; as RSA requires the message to be less than `n` (modulus). String is chunked in such a way that every substring is of the same length and all the substring's integer representation is less than `n`. The defination of `chunk_size` can be found [here](https://github.com/hyouteki/senet/blob/80fe406788627f34b35f5d4770a395764deefc91/pkda/pkda/rsa.h#L34).
- After the chunking, each substring is encoded into integer representation. Each character is encoded via 2 digit number and concatinated to form the integer representation. The encoding can be found [here](https://github.com/hyouteki/senet/blob/80fe406788627f34b35f5d4770a395764deefc91/pkda/pkda/rsa.h#L25-L45).

Encryption of each substring happens independently of each other and other connected via a counter mode (galois) just like the one in [AES/GCM](https://en.wikipedia.org/wiki/Galois/Counter_Mode) algorithm. So that two substrings containing same text should not lead to same encryption.

At the end of substring encryption the resultant is padded with leading zeroes to make it of same length as the `n`. Such that after concatination of all the substrings; a border can be drawn between substrings for decryption.

> Decryption happens in same way but in reverse.<br>
> __NOTE__: Using incorrect keys for encryption and decryption may lead to errors, as the implementation heavily relies on the encoding and using incorrect keys will break the encoding.<br>
> Implementation of RSA can be found [here](https://github.com/hyouteki/senet/blob/main/pkda/pkda/rsa.h).

### Guide
- __Generate keys__: This will generate keys of specified key size (default-2048) and print the relevant informations to the console (n, e: encryption key, and d: decryption key) also store the keys in json file (keys.json).
  ``` console
  ./rsa genkeys <key_size: optional; default-2048>
  ```
- __Encryption__: This will encrypt the provided text with the provided key.
  ``` console
  ./rsa encrypt "<text>" <e> <n>
  ```
- __Decryption__: This will decrypt the provided encrypted text with the provided key.
  ``` console
  ./rsa decrypt "<encrypted-text>" <d> <n>
  ```
  
## Quick start
- Install the required packages using `make install`.
- Build the project using `make`.

## Guide for PKDA
Run the pkda using `./runpkda` binary requried that the project is already built.

## Application
A simple encypted chat client is made. Two users [chatserver.c](https://github.com/hyouteki/senet/blob/main/pkda/chatserver.c) and [chatclient.c](https://github.com/hyouteki/senet/blob/main/pkda/chatclient.c) chats with each other. Chat is initiated by the `chatclient`. Both the users do not know each others public key for encryption and decryption. Thus they make requests with PKDA and receive each others public keys and proceeds with the chatting.

![Encrypted chat clients](https://github.com/hyouteki/senet/assets/108230497/d9bcb7f5-9429-42da-95e5-ee1a2b0fa415)

### Guide
- Open three seperate terminals in this [directory](https://github.com/hyouteki/senet/tree/main/pkda).
  - Launch PKDA on one using `./runpkda`.
  - Launch chatserver on another using `make runchatserver`.
  - And lastly run `make runchatclient` on the last terminal.
  - Voila!! you are done.
