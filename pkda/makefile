CFLAGS := -lgmp -ljansson

default: build

install:
	sudo apt install libgmp-dev libjansson-dev

build: rsa.c pkda.c client.c chatserver.c chatclient.c 
	gcc rsa.c -o rsa $(CFLAGS)
	gcc pkda.c -o runpkda $(CFLAGS) 
	gcc client.c -o client $(CFLAGS)
	gcc chatserver.c -o chatserver $(CFLAGS)	
	gcc chatclient.c -o chatclient $(CFLAGS)	

runchatserver:
	./chatserver ./user_dataset/user_0.json 1 6667 6668

runchatclient:
	./chatclient ./user_dataset/user_1.json 0 6668 6667

clean:
	$(RM) rsa runpkda client chatserver chatclient *.txt
