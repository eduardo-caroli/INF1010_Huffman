all: heap.c main.c
	clear
	gcc -Wall -o main main.c heap.c
	./main

encoder: heap.c encoder.c
	clear
	gcc -Wall -o encoder encoder.c heap.c
	./encoder

decoder: heap.c decoder.c
	clear
	gcc -Wall -o decoder decoder.c heap.c
	./decoder
