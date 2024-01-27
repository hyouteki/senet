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
  ;; storing hash in the str
  mov ebx, 19
.L2: 
  cmp ebx, -1
  je .L3
  mov rax, qword [hash]
  xor rdx, rdx
  mov rdi, 10
  div rdi
  mov qword [hash], rax
  add rdx, 48
  mov byte [hash.ptr+ebx], dl
  dec ebx
  jmp .L2
.L3:
  mov rax, 1
  mov rdi, 1
  mov esi, hash.ptr
  mov edx, 20
  syscall
  mov rax, 60
  xor rdi, rdi 
  syscall
  
section '.data' writable executable
plaintext.str db "hello world", 0
plaintext.len = $ - plaintext.str
plaintext.strlen = plaintext.len - 1
hash rq 1
msg db "Hash: %lu", 10, 0
hash.maxlen dd 20
hash.ptr rb 20 ;; = hash.maxlen
NEG_ONE dd -1
