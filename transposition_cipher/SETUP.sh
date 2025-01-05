#!/bin/bash
set -x -e

fasm_dir="fasm-1.73.32"
if [ ! -d $fasm_dir ]; then
	wget https://flatassembler.net/$fasm_dir.tgz
	tar -xzvf $fasm_dir.tgz
	rm $fasm_dir.tgz
fi
