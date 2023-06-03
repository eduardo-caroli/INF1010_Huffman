#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HEAP_SIZE 256
#define INT(c) ((int)c)
#define F(i) ((i-1)/2)
#define LC(i) ((2*i)+1)
#define RC(i) ((2*i)+2)

typedef int bool;
#define TRUE 1
#define FALSE 0

typedef struct _HeapNode{
	bool isChar;
	char c;
	int weight;
	struct _HeapNode *l;
	struct _HeapNode *r;
} HeapNode;

typedef struct {
	int pos;
	int max;
	HeapNode keys[MAX_HEAP_SIZE];
} Heap;

typedef struct _qNode{
	HeapNode *node;
	struct _qNode *next;
} qNode;

typedef qNode* Queue;

Heap newHeap();
int parse(char *pChars, int *cCount, const char *filename);
void swap(HeapNode *nodes, int pos1, int pos2);
Queue enq(HeapNode *newNode, Queue queue);
Queue buildQueue(char *pChars, int *cCount, int nChars);
Queue newQueue();
Queue createSubTree(Queue queue, bool *cont);
Queue deq(Queue queue, HeapNode **node);
HeapNode *newHeapNode(bool isChar, char character, int weight);
HeapNode *mergeNodes(HeapNode *left, HeapNode *right);
qNode *newQNode(HeapNode *node);
void printQ(Queue q);
void printTree(HeapNode *root);