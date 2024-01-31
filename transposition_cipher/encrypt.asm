format ELF64
  
section '.text' executable
include 'helper.asm'
public encrypt
  
encrypt:
  push.callee.save
  ;; accepting inputs
  str.copy plaintext.ptr, rdi
  mov dword [plaintext.len], esi
  mov dword [key.len], ecx
  intarray.copy key.ptr, rdx, dword [key.len]
  ;; initializing variables
  mov dword [plaintext.strlen], esi
  sub dword [plaintext.strlen], 1
  ;; calculating length of cipher text
  mov eax, dword [plaintext.strlen]
  xor rbx, rbx
  xor rdx, rdx
  mov ebx, dword [key.len]
  div rbx
  mov eax, dword [plaintext.strlen]
  add eax, dword [key.len]
  sub eax, edx
  mov dword [ciphertext.len], eax
  add dword [ciphertext.strlen], eax
  ;; aligning plaintext.ptr
  xor ebx, ebx
.L6:
  cmp ebx, dword [plaintext.strlen]
  je .L7
  mov dil, byte [plaintext.ptr+ebx]
  mov byte [plaintext.ptr+ebx], dil
  inc ebx
  jmp .L6
.L7:
  cmp ebx, dword [ciphertext.strlen]
  je .L8
  mov byte [plaintext.ptr+ebx], 'z' ; padding character = 'z'
  inc ebx
  jmp .L7
.L8:
  mov byte [plaintext.ptr+ebx], 0
  ;; setting up ciphertext
  mov ebx, dword [ciphertext.strlen]  
  mov byte [ciphertext.ptr+ebx], 0
  mov ecx, 1 ; current_column
  xor edx, edx ; ciphertext_len
.L1:
  cmp ecx, dword [key.len]
  jg .L4
  xor ebx, ebx ; offset for the current_column in the plaintext
.L2: ; calculating offset for current column
  cmp ecx, dword [key.ptr + 4*ebx]
  je .L3
  inc ebx
  jmp .L2
.L3: ; pushing to ciphertext
  cmp ebx, dword [ciphertext.len]
  jge .L5
  mov dil, byte [plaintext.ptr+ebx]
  mov byte [ciphertext.ptr+edx], dil 
  add ebx, dword [key.len]
  inc edx
  jmp .L3
.L5:
  inc ecx
  jmp .L1
.L4: ; return
  mov rax, ciphertext.ptr
  pop.callee.save
  ret

section '.data' writeable executable
;; compile time constants
MAX_LEN equ 256
;; reserved for inputs
plaintext.ptr rb MAX_LEN
plaintext.len rd 1
key.ptr rd MAX_LEN
key.len rd 1
;; variables
plaintext.strlen rd 1
ciphertext.strlen rd 1
;; reserved for outputs
ciphertext.ptr rb 256
ciphertext.len rd 1
