format ELF64
 
section '.text' executable

macro exit code {
  mov rax, 60
  mov rdi, code
  syscall
}

macro exit_e code {
  mov rax, 60
  mov edi, code
  syscall
}

macro write text, text_len {
  mov rax, 1
  mov rdi, 1
  mov esi, text
  mov edx, text_len
  syscall
}
  
struc str x {
  .str db x, 0
  .len = $ - .str
  .strlen = .len - 1
}
  
public _start
_start:
  ;; calculating length of cipher text
  mov rax, plaintext.strlen
  xor rbx, rbx
  xor rdx, rdx
  mov ebx, dword [key.len]
  div rbx
  mov eax, plaintext.strlen
  add eax, dword [key.len]
  sub eax, edx ; now eax contains ciphertext.strlen
  mov dword [ciphertext.len], eax
  add dword [ciphertext.strlen], eax

  ;; aligning plaintext.ptr
  mov ebx, 0

.L6:
  cmp ebx, plaintext.strlen
  je .L7
  mov dil, byte [plaintext.str+ebx]
  mov byte [plaintext.ptr+ebx], dil
  inc ebx
  jmp .L6
  
.L7:
  cmp ebx, dword [ciphertext.strlen]
  je .L8
  mov byte [plaintext.ptr+ebx], '-'
  inc ebx
  jmp .L7

.L8:
  mov byte [plaintext.ptr+ebx], 0
  
  ;; setting up ciphertext
  mov ebx, dword [ciphertext.strlen]  
  mov byte [ciphertext.ptr+ebx], 0
  mov ecx, 1 ; current_column
  mov edx, 0 ; ciphertext_len
  
.L1:
  cmp ecx, dword [key.len]
  jg .L4
  mov ebx, 0 ; offset for the current_column in the plaintext

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
  
.L4: ; printing ciphertext
  write ciphertext.ptr, edx
  exit 0

.L5:
  inc ecx
  jmp .L1

section '.data' writeable executable
plaintext.ptr rb 256
plaintext str "hello world"
; plaintext str "hey you there can we take it to the next level baby do you dare - Naked by James Arthur"
pt_len_t dd 0
ciphertext.ptr rb 256 ; max ciphertext.len = 256
ciphertext.len dd 0
ciphertext.strlen dd 1
key.ptr dd 2, 3, 1, 4, 6, 5, 7 
key.len dd 7
