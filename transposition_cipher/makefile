all: hash enc

hash: hash.asm hash.test.asm
	fasm hash.asm
	fasm hash.test.asm
	ld hash.test.o hash.o -dynamic-linker /lib64/ld-linux-x86-64.so.2 -lc -o hash

encrypt: encrypt.asm encrypt.test.asm
	fasm encrypt.asm
	fasm encrypt.test.asm
	ld encrypt.test.o encrypt.o -dynamic-linker /lib64/ld-linux-x86-64.so.2 -lc -o encrypt

decrypt: decrypt.asm decrypt.test.asm
	fasm decrypt.asm
	fasm decrypt.test.asm
	ld decrypt.test.o decrypt.o -dynamic-linker /lib64/ld-linux-x86-64.so.2 -lc -o decrypt

clean:
	$(RM) *.o encrypt decrypt hash
