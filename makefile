all: encoder decoder 
	clear
	make encoder
	make decoder
	./encoder exemplo.txt arquivo.cmp
	./decoder arquivo.cmp transcrito.txt
	open transcrito.txt

encoder: heap.c encoder.c
	clear
	gcc -Wall -o encoder encoder.c heap.c

decoder: heap.c decoder.c
	clear
	gcc -Wall -o decoder decoder.c heap.c
