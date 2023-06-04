#include "heap.h"

int main(void) {
	int nNodes, nChars;
	FILE *binaryFile;
	FILE *textFile;
	binaryFile = fopen("exemplo.cmp", "rb");
	textFile = fopen("translated.txt", "w");
	fread(&nNodes, sizeof(int), 1, binaryFile);
	printf("nNodes: %d\n", nNodes);
	HeapNode huffmanTree[nNodes];
	fread(huffmanTree, sizeof(HeapNode), nNodes, binaryFile);
	printArrTree(huffmanTree, 0);
	nChars = huffmanTree[0].weight;
	printf("==> %d\n", nChars);
	while(nChars > 0) {
		//funcao que recebe o bitstream biario, o de texto e a arvore e traduz de um para o outro
		translateSingleCharacter(binaryFile, textFile, huffmanTree);
		nChars--;
	}

	fclose(binaryFile);
	fclose(textFile);
	return 0;
}
