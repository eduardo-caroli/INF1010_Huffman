#include "heap.h"

Heap newHeap() {
	Heap newHeap;
	newHeap.max = MAX_HEAP_SIZE;
	return newHeap;
}

//essa funcao espera que array seja um vetor com espaco o suficiente
//o espaco necessario para representar uma arvore de HeapNode com alocacao dinamica pode ser obtido por meio da funcao numNodes
//chame essa funcao passando pos igual a 0 para converter a arvore inteira
void makeArrayRepresentation(HeapNode* tree, HeapNode *array, int pos) {
	array[pos] = *tree;
	printf("pos: %d -> %c : %d\n", pos, array[pos].isChar ? array[pos].c : '@', array[pos].weight);
	if(tree->isChar) return;
	makeArrayRepresentation(tree->l, array, LC(pos));
	makeArrayRepresentation(tree->r, array, RC(pos));
	return;
}

HeapNode *mergeNodes(HeapNode *left, HeapNode *right) {
	int sum = right->weight + left->weight; 
	HeapNode *newNode;
	newNode = newHeapNode(FALSE, 0, sum);
	newNode->l = left;
	newNode->r = right;
	return newNode;
}

HeapNode *newHeapNode(bool isChar, char character, int weight) {
	HeapNode *newNode;
	newNode = (HeapNode*)malloc(sizeof(HeapNode));
	if(newNode == NULL) {
		fprintf(stderr, "Impossivel alocar espaco. Abortando");
		exit(1);
	}
	if (isChar) {
		newNode->c = character;
	}
	newNode->weight = weight;
	newNode->isChar = isChar;
	newNode->l = NULL;
	newNode->r = NULL;
	return newNode;
}

qNode *newQNode(HeapNode *node) {
	qNode *newNode = (qNode*)malloc(sizeof(qNode));
	if(newNode == NULL) {
		fprintf(stderr, "Impossivel alocar espaco. Abortando");
		exit(2);
	}
	newNode->node = node;
	newNode->next = NULL;
	return newNode;
}

void swap(HeapNode *nodes, int pos1, int pos2) {
	HeapNode aux = nodes[pos1];
	nodes[pos1] = nodes[pos2];
	nodes[pos2] = aux;
}

void moveUp(Heap heap, int pos) {
	int childKey, parentKey;
	childKey = heap.keys[pos].weight; 
	parentKey = heap.keys[F(pos)].weight; 
	if(pos == 0) return;
	if( parentKey <= childKey) return;
	swap(heap.keys, childKey, parentKey);
	moveUp(heap, F(pos));
}

void moveDown(Heap heap, int pos) {
	int lKey, rKey, minPos, parentKey;
	if(LC(pos) >= heap.pos) return; //se e folha, retorna
	lKey = heap.keys[LC(pos)].weight;
	minPos = LC(pos);
	if(RC(pos) < heap.pos) { //tem filho direito
		rKey = heap.keys[RC(pos)].weight;
		minPos = (lKey < rKey) ? LC(pos) : RC(pos);
	}
	if(heap.keys[pos].weight > heap.keys[minPos].weight) {
		swap(heap.keys, pos, minPos);
		moveDown(heap, minPos);
	}
	return;
}

void printTree(HeapNode *root) {
	if(root == NULL) return;	
	if(root->isChar) {
		printf("%c : %d\n", root->c, root->weight);
	} else {
		printf("no vazio : %d\n", root->weight);
	}
	printTree(root->l);
	printTree(root->r);
	return;
}

Queue createSubTree(Queue queue, bool *cont) {
	HeapNode *first, *second, *merged;
	Queue q;
	q = deq(queue, &first);
	if (q == NULL) {
		*cont = FALSE;
		q = enq(first, q);
		return q;
	}
	q = deq(q, &second);
	merged = mergeNodes(first, second);
	q = enq(merged, q);
	if(q==NULL) {printf("wrong here\n");}
	return q;
}

Queue buildQueue(char *pChars, int *cCount, int nChars) {
	int i;
	HeapNode *currNode;
	Queue q = newQueue();
	for(i = 0; i < nChars; i++) {
		currNode = newHeapNode(TRUE, pChars[i], cCount[INT(pChars[i])]);
		q = enq(currNode, q);
	}
	return q;
}

Queue newQueue() {
	return NULL;
}

Queue deq(Queue queue, HeapNode **node) {
	*node = queue->node;
	qNode *aux = queue->next;
	free(queue);
	return aux;
}

int height(HeapNode *tree) {
	int maxHeight, lHeight, rHeight;
	if (tree == NULL) return 0;
	lHeight = height(tree->l);
	rHeight = height(tree->r);
	maxHeight = lHeight > rHeight ? lHeight : rHeight;
	return 1  + maxHeight;
}

int parse(char *pChars, int *cCount, const char *filename) {
	char c, *presentChars;
	int *charCount, total = 0;
	FILE *file = fopen(filename, "r");

	presentChars = (char*)malloc(256 * sizeof(char));
	charCount = (int*)calloc(256, sizeof(int));

	if ((charCount == NULL) || (presentChars == NULL)){
		fprintf(stderr, "Impossivel alocar espaco. Abortando");
		exit(1);
	}

	while (fscanf(file, "%c", &c) == 1) {
		if (charCount[INT(c)] == 0) {
			presentChars[total] = c;
			total++;
		}
		charCount[INT(c)] += 1;
	}
	memcpy(pChars, presentChars, total);
	memcpy(cCount, charCount, 256 * sizeof(int));
	free(presentChars);
	free(charCount);
	fclose(file);
	return total;
}

Queue enq(HeapNode *newNode, Queue queue) {
	qNode *new;
	if (queue == NULL) { 
		new = newQNode(newNode);
		return new;
	}
	if(newNode == NULL) printf("newNode is NULL\n");
	if(queue == NULL) printf("queue is NULL\n");
	if(queue->node == NULL) printf("queue->node is NULL\n");
	if (newNode->weight == queue->node->weight) {
		if (queue->node->isChar) { //se o no analisado e um caracter
			if(newNode->isChar) { //se o no a inserir e um caracter
				if(newNode->c < queue->node->c) { //se o no a inserir e lexicograficamente inferior ao no atual
					goto insertNewNode;
				} else {
					goto next;
				}
			} else { //se o no a inserir e um no raiz (sem caracter)
				goto insertNewNode;
			}
		} else { //se o no analisado e um no raiz (sem caracter)
			if(newNode->isChar) {
				goto next;
			} else {
				goto next;
			}
		}
	}

	if (newNode->weight < queue->node->weight) {
	insertNewNode:
		 new = newQNode(newNode);
		 new->next = queue;
		 return new;
	}
	next:
		queue->next = enq(newNode, queue->next);
		return queue;
}

void printQ(Queue q) {
	if(q == NULL) return;
	if(q->node->isChar) {
		printf("%c - %d\n", q->node->c, q->node->weight);
	} else {
		printf("vazio - %d\n", q->node->weight);
	}
	printQ(q->next);
	return;
}

void printArrTree(HeapNode tree[], int pos) {
	printf("->pos: %d -> %c : %d\n", pos, tree[pos].c, tree[pos].weight);
	if(tree[pos].isChar) return;	
	if(tree->l != NULL) printArrTree(tree, LC(pos));
	if(tree->r != NULL) printArrTree(tree, RC(pos));
}
