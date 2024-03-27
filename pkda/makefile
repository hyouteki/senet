default: build

install:
	sudo apt install libgmp-dev libjansson-dev

build: rsa.c pkda.c client.c
	gcc rsa.c -o rsa -lgmp -ljansson
	gcc pkda.c -o runpkda -ljansson -lgmp
	gcc client.c -o client -ljansson

clean:
	$(RM) rsa runpkda client *.txt
