format ELF64
  
section '.text' executable
include 'helper.asm'
public decrypt

decrypt:
  ;; accepting inputs
  str.copy ciphertext.ptr, rdi
  mov dword [ciphertext.len], esi
  mov dword [key.len], r10d
  intarray.copy key.ptr, rdx, dword [key.len]
  ;; init ciphertext.strlen
  mov dword [ciphertext.strlen], esi
  sub dword [ciphertext.strlen], 1
  ;; init cycles
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
decrypt.end: ; removing trailing '-'(s)
  dec rbx
decrypt.trailing.dash.loop:
  cmp byte [plaintext.ptr+rbx], '-'
  jne decrypt.trailing.dash.end
  dec rbx
  jmp decrypt.trailing.dash.loop
decrypt.trailing.dash.end:
  inc rbx
  mov byte [plaintext.ptr+rbx], 0
  mov rax, plaintext.ptr
  inc rbx
  mov r12d, ebx
  ret
  
section '.data' writable executable
;; compile time constants
MAX_LEN equ 256
KEY_LEN equ 7
;; variables
plaintext.itr rq 1
column dd 1
cycles dd 1
;; reserved for inputs
ciphertext.ptr rb MAX_LEN
ciphertext.len rd 1
ciphertext.strlen rd 1
key.ptr rd KEY_LEN
key.len dd KEY_LEN
;; reserved for outputs
plaintext.ptr rb MAX_LEN