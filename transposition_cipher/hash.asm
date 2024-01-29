format ELF64
  
section '.text' executable  
include 'helper.asm'
public hash
  
hash:
  push.callee.save
  ;; accepting inputs
  mov dword [plaintext.strlen], esi
  str.copy plaintext.str, rdi
  ;; initializing variables
  xor ebx, ebx
  mov qword [hash.val], 5381
.L0:
  ;; hash = (hash * 33) ^ char
  cmp ebx, dword [plaintext.strlen]
  je .L1
  mov rcx, qword [hash.val]
  shl qword [hash.val], 5
  add qword [hash.val], rcx
  xor rcx, rcx
  mov cl, byte [plaintext.str+ebx]
  xor qword [hash.val], rcx
  inc ebx
  jmp .L0
.L1:
  ;; storing hash in the str
  mov ebx, 19
.L2: 
  cmp ebx, -1
  je .L3
  mov rax, qword [hash.val]
  xor rdx, rdx
  mov rdi, 10
  div rdi
  mov qword [hash.val], rax
  add rdx, 48
  mov byte [hash.ptr+ebx], dl
  dec ebx
  jmp .L2
.L3: ; return
  mov rax, hash.ptr
  pop.callee.save
  ret
  
section '.data' writable executable
;; compile time constants
PLAINTEXT.MAX_LEN equ 256
HASH.LEN equ 20
;; reserved for inputs
plaintext.str rb PLAINTEXT.MAX_LEN
plaintext.strlen rd 1
;; variables
hash.val rq 1
;; reserved for output
hash.ptr rb HASH.LEN 
