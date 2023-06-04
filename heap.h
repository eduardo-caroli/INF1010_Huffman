#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_HEAP_SIZE 256
#define INT(c) ((int)c)
#define F(i) ((i-1)/2)
#define LC(i) ((2*i)+1)
#define RC(i) ((2*i)+2)

typedef int bool;
#define TRUE 1
#define FALSE 0

typedef struct _HuffmanNode{
	bool isChar;
	char c;
	int weight;
	struct _HuffmanNode *l;
	struct _HuffmanNode *r;
} HuffmanNode;

typedef struct {
	int pos;
	int max;
	HuffmanNode keys[MAX_HEAP_SIZE];
} Heap;

typedef struct _QueueNode{
	HuffmanNode *node;
	struct _QueueNode *next;
} QueueNode;

typedef struct {
	int bits;
	int bitCount;
} BitPattern;

typedef QueueNode* Queue;

Heap newHeap();
int parse(char *pChars, int *cCount, const char *filename);
int height(HuffmanNode *tree);
void swap(HuffmanNode *nodes, int pos1, int pos2);
Queue enq(HuffmanNode *newNode, Queue queue);
Queue buildQueue(char *pChars, int *cCount, int nChars);
Queue newQueue();
Queue createSubTree(Queue queue, bool *cont);
Queue deq(Queue queue, HuffmanNode **node);
HuffmanNode *newHuffmanNode(bool isChar, char character, int weight);
HuffmanNode *mergeNodes(HuffmanNode *left, HuffmanNode *right);
void makeArrayRepresentation(HuffmanNode* tree, HuffmanNode *array, int pos);
QueueNode *newQNode(HuffmanNode *node);
void printQ(Queue q);
void printTree(HuffmanNode *root);
void printArrTree(HuffmanNode *tree, int pos);
void writeBit(int bit, FILE *file, bool forceWrite);
int readBit(FILE *file);
void makeBitPatternTable(HuffmanNode *huffmanTree, BitPattern *table, int pattern, int bitCount);
void encode(BitPattern *table, FILE *file);
void writeBitPattern(char c, BitPattern *table, FILE *file, bool isLastChar);
void translateSingleCharacter(FILE *binaryFile, FILE *textFile, HuffmanNode *tree);
