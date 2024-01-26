all: main.o test.o

main: main.o
	ld main.o -dynamic-linker /lib64/ld-linux-x86-64.so.2 -lc -o main

main.o: main.asm
	fasm main.asm

hash: hash.o
	ld hash.o -dynamic-linker /lib64/ld-linux-x86-64.so.2 -lc -o hash

hash.o: hash.asm
	fasm hash.asm

clean:
	$(RM) *.o
