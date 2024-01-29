all: hash encrypt decrypt

hash: hash.asm hash.test.asm
	fasm hash.asm
	fasm hash.test.asm
	ld hash.test.o hash.o -dynamic-linker /lib64/ld-linux-x86-64.so.2 -lc -o hash

encrypt: encrypt.asm encrypt.test.asm
	fasm encrypt.asm
	fasm encrypt.test.asm
	ld encrypt.test.o encrypt.o -dynamic-linker /lib64/ld-linux-x86-64.so.2 -lc -o encrypt

brute:
	fasm decrypt.asm decrypt.o
	fasm hash.asm hash.o
	gcc -no-pie brute.c decrypt.o hash.o -o brute
	./brute

clean:
	$(RM) *.o encrypt decrypt hash brute
