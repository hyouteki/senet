build: decrypt.asm encrypt.asm hash.asm transposition_cipher.c
	fasm decrypt.asm decrypt.o
	fasm encrypt.asm encrypt.o
	fasm hash.asm hash.o
	gcc -no-pie transposition_cipher.c decrypt.o encrypt.o hash.o -o transposition_cipher

clean:
	$(RM) *.o transposition_cipher
