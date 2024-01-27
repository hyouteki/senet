macro exit code {
  mov rax, 60
  mov edi, code
  syscall  
}

macro str.copy dst, src {
  push rsi
  push rdi
  push rax
  mov rsi, src
  mov rdi, dst
str.copy.loop:
  lodsb
  stosb
  test al, al
  jnz str.copy.loop
str.copy.end:
  pop rax
  pop rdi
  pop rsi
}

macro str.print str, len {
  push rax
  push rdi
  push rsi
  push rdx
  mov rax, 1
  mov rdi, 1
  mov rsi, str
  mov edx, len
  syscall
  pop rdx
  pop rsi
  pop rdi
  pop rax 
}

macro intarray.copy dst, src, len {
  push rsi
  push rdi
  push rbx
  push rcx
  mov rsi, src
  mov rdi, dst
  xor ebx, ebx	  
intarray.copy.loop:
  cmp ebx, len
  je intarray.copy.end
  mov ecx, dword [rsi]
  mov dword [rdi], ecx
  add rsi, 4
  add rdi, 4
  inc ebx
  jmp intarray.copy.loop
intarray.copy.end:
  pop rcx
  pop rbx
  pop rdi
  pop rsi
}
