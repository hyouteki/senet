format ELF64
  
section '.text' executable  
include 'helper.asm'
public hash
  
hash:
  push.callee.save
  ;; accepting inputs
  str.copy plaintext.str, rdi
  ;; initializing variables
  xor ebx, ebx
  mov rax, 5381
  mov rdi, 33
hash.loop:
  cmp byte [plaintext.str+ebx], 0
  je hash.end
  xor rdx, rdx
  mul rdi
  xor al, byte [plaintext.str+ebx]
  inc ebx
  jmp hash.loop
hash.end:
  pop.callee.save
  ret
  
section '.data' writable executable
;; compile time constants
PLAINTEXT.MAX_LEN equ 256
;; reserved for inputs
plaintext.str rb PLAINTEXT.MAX_LEN
