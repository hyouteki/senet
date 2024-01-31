format ELF64

section '.text'
include 'helper.asm'
public decrypt
  
decrypt:
  push.callee.save
  ;; accepting inputs
  str.copy ciphertext.ptr, rdi
  mov dword [ciphertext.len], esi
  mov dword [key.len], ecx
  intarray.copy key.ptr, rdx, dword [key.len]
  ;; init ciphertext.strlen
  mov dword [ciphertext.strlen], esi
  sub dword [ciphertext.strlen], 1
  ;; init cycles
  mov dword [column], 1
  mov eax, dword [ciphertext.strlen]
  xor rdx, rdx
  mov ebx, [key.len]
  div ebx
  mov dword [cycles], eax
  ;; init ciphertext.itr
  xor rbx, rbx
decrypt.loop:
  cmp ebx, dword [ciphertext.strlen]
  jge decrypt.end
  intarray.getindex key.ptr, dword [column], dword [key.len]
  mov rcx, rax ; plaintext.itr
  xor rdx, rdx ; cur_cycle_count
decrypt.plaintext.append.loop:
  cmp edx, dword [cycles]
  jge decrypt.plaintext.append.end
  mov dil, byte [ciphertext.ptr+rbx+rdx]
  mov byte [plaintext.ptr+rcx], dil
  inc edx
  add ecx, dword [key.len]
  jmp decrypt.plaintext.append.loop
decrypt.plaintext.append.end:
  add ebx, dword [cycles]
  inc dword [column]
  jmp decrypt.loop
decrypt.end: ; removing padding
  dec rbx
decrypt.padding.loop:
  cmp byte [plaintext.ptr+rbx], 'z' ; padding character = 'z'
  jne decrypt.padding.end
  dec rbx
  jmp decrypt.padding.loop
decrypt.padding.end:
  inc rbx
  mov byte [plaintext.ptr+rbx], 0
  mov rax, plaintext.ptr
  pop.callee.save
  ret
  
section '.data'
;; compile time constants
MAX_LEN equ 256
MAX_KEY_LEN equ 9
;; variables
plaintext.itr rq 1
column rd 1
cycles dd 1
;; reserved for inputs
ciphertext.ptr rb MAX_LEN
ciphertext.len rd 1
ciphertext.strlen rd 1
key.ptr rd MAX_KEY_LEN
key.len rd 1
;; reserved for outputs
plaintext.ptr rb MAX_LEN
