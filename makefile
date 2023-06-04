all: encoder decoder 
	clear
	make encoder
	make decoder
	./encoder exemplo.txt arquivo.cmp
	./decoder arquivo.cmp transcrito.txt
	open transcrito.txt

encoder: aux.c encoder.c
	clear
	gcc -Wall -o encoder encoder.c aux.c

decoder: aux.c decoder.c
	clear
	gcc -Wall -o decoder decoder.c aux.c
