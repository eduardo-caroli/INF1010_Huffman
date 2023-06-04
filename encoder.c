#include "heap.h"

int main(int agrc, char *argv[]) {

	//nesse array, esta um numero de caracteres igual a numberOfDistinctChars
	//os quais sao os caracteres presentes no arquivo texto fonte
	char lastReadCharacter, presentChars[256]; 

	//essa tabela contem os padroes de bits para os caracteres presentes no arquivo fonte
	BitPattern bitPatternTable[256];

	//numberOfDistinctChars e o numero de caracteres diferentes presentes no arquivo fonte
	//charOcurrenceTable e uma tabela que relaciona um caracter com o numero de vezes em que ocorre
	//no arquivo
	int charsLeftToWrite, numberOfDistinctChars, charOcurrenceTable[256];

	//esta e a fila que sera usada para montar a arvore de huffman
	Queue q;
	bool keepBuildingTree = TRUE;
	FILE *binaryFile, *textFile;
	textFile = fopen("exemplo.txt", "r");
	binaryFile = fopen("exemplo.cmp", "wb");
	numberOfDistinctChars = parse(presentChars, charOcurrenceTable, "exemplo.txt");
	q = buildQueue(presentChars, charOcurrenceTable, numberOfDistinctChars);
	while(keepBuildingTree) {
		q = createSubTree(q, &keepBuildingTree);
	}
	charsLeftToWrite = q->node->weight;
	int h = height(q->node);
	int nNodes = (int)pow(2, h);
	HeapNode baum[nNodes];
	makeArrayRepresentation(q->node, baum, 0);
	makeBitPatternTable(q->node,bitPatternTable, 0, 0); 
	fwrite(&nNodes, sizeof(int), 1, binaryFile);
	fwrite(baum, sizeof(HeapNode), nNodes, binaryFile);
	//agora vamos codificar o arquivo texto
	printf("number of characters: %d\n", charsLeftToWrite);
	while(fscanf(textFile, "%c", &lastReadCharacter) == 1) {
		writeBitPattern(lastReadCharacter, bitPatternTable, binaryFile, (charsLeftToWrite == 1));
		charsLeftToWrite--;
	}
	fclose(binaryFile);
}
