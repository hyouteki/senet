format ELF64

section '.text' executable
include 'helper.asm'
extrn decrypt
public _start

_start:
  mov rdi, ciphertext.ptr
  mov esi, ciphertext.len
  mov rdx, key.ptr
  mov r10d, dword [key.len]
  call decrypt
  mov rbx, rax
  str.copy plaintext.ptr, rbx
  mov dword [plaintext.len], r12d
  str.print plaintext.ptr, dword [plaintext.len]
  exit 0
  
section '.data' writable executable
;; compile time constants
MAX_LEN equ 256
;; variables
ciphertext.ptr db "llHoerld -o-w-", 0
ciphertext.len = $ - ciphertext.ptr
key.ptr dd 2, 3, 1, 4, 6, 5, 7
key.len dd 7
plaintext.ptr rb MAX_LEN
plaintext.len rd 1
