all: heap.c main.c
	clear
	gcc -Wall -o main main.c heap.c
	./main
