#!/bin/bash
set -x -e

FASM=fasm/fasm
ASM_FILES=("decrypt" "encrypt" "hash")

for file in "${ASM_FILES[@]}"; do
	$FASM $file.asm $file.o
done
gcc -no-pie transposition_cipher.c "${ASM_FILES[@]/%/.o}" -o transposition_cipher
rm *.o
