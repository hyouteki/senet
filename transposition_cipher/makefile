all: hash enc

hash: hash.asm hash.test.asm
	fasm hash.asm
	fasm hash.test.asm
	ld hash.test.o hash.o -dynamic-linker /lib64/ld-linux-x86-64.so.2 -lc -o hash

enc: enc.asm enc.test.asm
	fasm enc.asm
	fasm enc.test.asm
	ld enc.test.o enc.o -dynamic-linker /lib64/ld-linux-x86-64.so.2 -lc -o enc

clean:
	$(RM) *.o enc hash
