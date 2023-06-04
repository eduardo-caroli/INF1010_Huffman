#include "heap.h"
#define TEST_PARSING

int main(void) {
	#ifdef TEST_NODE_CREATION
	char c; int i;
	Queue q = NULL;
	HeapNode *newNode;
	while(TRUE) {
		printf("char: ");
		scanf("%c", &c);
		printf("count: ");
		scanf("%d", &i);
		newNode = newHeapNode(TRUE, c, i);
		printf("newNode: %c, %d\n", newNode->c, newNode->weight);
		q = enq(newNode, q);
		printQ(q);
		fflush(stdin);
	}
	#endif

	#ifdef TEST_PARSING
	char c[256];
	BitPattern bitPatternTable[256];
	int cCount, charCount[256];
	Queue q;
	bool cont = TRUE;
	cCount = parse(c, charCount, "exemplo.txt");
	printf("\n");
	q = buildQueue(c, charCount, cCount);
	printQ(q);
	while(cont) {
		q = createSubTree(q, &cont);
	}
	int h = height(q->node);
	int nNodes = (int)pow(2, h);
	HeapNode baum[nNodes];
	makeArrayRepresentation(q->node, baum, 0);
	makeBitPatternTable(q->node,bitPatternTable, 0, 0); 
	FILE *bin = fopen("exemplo.bin", "wb");
	fwrite(&nNodes, 1, sizeof(int), bin);
	fwrite(baum, nNodes, sizeof(HeapNode), bin);
	for(int count = 0; count < 8; count++) {
		writeBit(count % 2, bin, FALSE);
	}
	writeBit(1, bin, TRUE);
	fclose(bin);
	bin = fopen("exemplo.bin", "rb");
	fread(&nNodes, 1, sizeof(int), bin);
	HeapNode arbol[nNodes];
	fread(arbol, nNodes, sizeof(HeapNode), bin);
	#endif

	return 0;
}
