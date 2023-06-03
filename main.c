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
	int cCount, charCount[256];
	Queue q;
	bool cont = TRUE;
	cCount = parse(c, charCount, "exemplo.txt");
	printf("\n");
	q = buildQueue(c, charCount, cCount);
	printQ(q);
	while(cont) {
		printf("================================\n");
		q = createSubTree(q, &cont);
		printQ(q);
	}
	if(q->node == NULL) printf("I really do become NULL");
	printTree(q->node);
	int h = height(q->node);
	printf("height: %d\n", h);
	int nNodes = (int)pow(2, h);
	HeapNode baum[nNodes];
	makeArrayRepresentation(q->node, baum, 0);
	printArrTree(baum, 0);
	#endif

	return 0;
}
