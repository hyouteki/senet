format ELF64

section '.text' executable
include 'helper.asm'
extrn encrypt
public _start

_start:
  mov rdi, plaintext.ptr
  mov esi, plaintext.len
  mov rdx, key.ptr
  mov r10d, dword [key.len]
  call encrypt
  mov r8, rax
  str.copy ciphertext.ptr, r8
  mov dword [ciphertext.len], r12d
  str.print ciphertext.ptr, dword [ciphertext.len]
  exit 0
  
section '.data' writable executable
;; compile time constants
MAX_LEN equ 256
;; variables
plaintext.ptr db "Hello world", 0
plaintext.len = $ - plaintext.ptr
key.ptr dd 2, 3, 1, 4, 6, 5, 7
key.len dd 7
ciphertext.ptr rb MAX_LEN
ciphertext.len rd 1 
