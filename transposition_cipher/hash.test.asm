format ELF64
  
section '.text' executable  
extrn hash

macro str.copy src, dst {
  mov rsi, src
  mov rdi, dst
copy_loop:
  lodsb
  stosb
  test al, al
  jnz copy_loop
}
  
public _start
  
_start:
  mov rdi, plaintext.str
  mov rsi, plaintext.strlen
  call hash
  str.copy rax, hash.ptr
  mov rax, 1
  mov rdi, 1
  mov rsi, hash.ptr
  mov rdx, 20
  syscall
  mov rax, 60
  mov rdi, 0
  syscall
  
section '.data' writable executable
;; compile time constants
HASH.LEN equ 20
;; variables
plaintext.str db "Hello world", 0
plaintext.strlen = $ - plaintext.str - 1
hash.ptr rb HASH.LEN 
