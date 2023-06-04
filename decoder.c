#include "heap.h"

int main(int argc, char *argv[]) {
	int nNodes, nChars;
	FILE *binaryFile;
	FILE *textFile;
	binaryFile = fopen(argv[1], "rb");
	if(binaryFile == NULL) {
		fprintf(stderr, "Impossivel abrir o arquivo binario solicitado\n");
		exit(2);
	}
	textFile = fopen(argv[2], "w");
	if(textFile == NULL) {
		fprintf(stderr, "Impossivel abrir o arquivo texto solicitado\n");
		exit(3);
	}
	fread(&nNodes, sizeof(int), 1, binaryFile);
	HeapNode huffmanTree[nNodes];
	fread(huffmanTree, sizeof(HeapNode), nNodes, binaryFile);
	nChars = huffmanTree[0].weight;
	while(nChars > 0) {
		//funcao que recebe o bitstream biario, o de texto e a arvore e traduz de um para o outro
		translateSingleCharacter(binaryFile, textFile, huffmanTree);
		nChars--;
	}

	fclose(binaryFile);
	fclose(textFile);
	return 0;
}
