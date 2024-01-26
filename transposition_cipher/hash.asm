format ELF64
  
section '.text' executable  
extrn printf

public _start
_start:
  xor ebx, ebx
  mov qword [hash], 5381
.L0:
  ;; hash = (hash * 33) ^ char
  cmp ebx, plaintext.strlen
  je .L1
  mov rcx, qword [hash]
  shl qword [hash], 5
  add qword [hash], rcx
  xor rcx, rcx
  mov cl, byte [plaintext.str+ebx]
  xor qword [hash], rcx
  inc ebx
  jmp .L0
.L1:
  xor rax, rax
  mov rdi, msg
  mov rsi, qword [hash]
  call printf
  mov rax, 60
  xor rdi, rdi 
  syscall
  
section '.data' writable executable
plaintext.str db "hello world", 0
plaintext.len = $ - plaintext.str
plaintext.strlen = plaintext.len - 1
hash rq 1
msg db "Hash: %lu", 10, 0
