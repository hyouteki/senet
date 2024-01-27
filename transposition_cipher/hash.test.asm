format ELF64
  
section '.text' executable  
include 'helper.asm'
extrn hash
public _start
  
_start:
  mov rdi, plaintext.str
  mov rsi, plaintext.strlen
  call hash
  mov r11, rax
  str.copy hash.ptr, r11
  str.print hash.ptr, 20
  exit 0
  
section '.data' writable executable
;; compile time constants
HASH.LEN equ 20
;; variables
plaintext.str db "Hello world", 0
plaintext.strlen = $ - plaintext.str - 1
hash.ptr rb HASH.LEN 
