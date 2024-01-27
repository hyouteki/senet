all: main.o hash.o

main: main.o
	ld main.o -dynamic-linker /lib64/ld-linux-x86-64.so.2 -lc -o main

main.o: main.asm
	fasm main.asm

hash: hash.o hash.test.o
	ld hash.test.o hash.o -dynamic-linker /lib64/ld-linux-x86-64.so.2 -lc -o hash

hash.o: hash.asm hash.test.asm
	fasm hash.asm
	fasm hash.test.asm

clean:
	$(RM) *.o main hash
