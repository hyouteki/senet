main: main.o
	ld main.o -dynamic-linker /lib64/ld-linux-x86-64.so.2 -lc -o main

main.o: main.asm
	fasm main.asm

clean:
	$(RM) *.o
