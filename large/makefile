RM := $(if $(filter Windows_NT, $(OS)), del /Q /F, rm -f)

big_number.test: big_number.test.cpp big_number.h
	g++ -O3 -Wall big_number.test.cpp -o big_number.test
	./big_number.test 123 4560 5656

diffie_hellman: diffie_hellman.cpp big_number.h prime_number.h
	g++ -O3 -Wall diffie_hellman.cpp -o diffie_hellman.test
	./diffie_hellman.test

large.test: large.test.c large.h
	gcc -O3 -Wall large.test.c -o large.test
	./large.test

clean:
	$(RM) *.test
