#include "heap.h"
#define showBitStream

Heap newHeap() {
	Heap newHeap;
	newHeap.max = MAX_HEAP_SIZE;
	return newHeap;
}

//essa funcao espera que array seja um vetor com espaco o suficiente
//o espaco necessario para representar uma arvore de HuffmanNode com alocacao dinamica pode ser obtido por meio da funcao numNodes
//chame essa funcao passando pos igual a 0 para converter a arvore inteira
void makeArrayRepresentation(HuffmanNode* tree, HuffmanNode *array, int pos) {
	array[pos] = *tree;
	if(tree->isChar) return;
	makeArrayRepresentation(tree->l, array, LC(pos));
	makeArrayRepresentation(tree->r, array, RC(pos));
	return;
}

//essa funcao recebe dois HuffmanNodes, correspondentes a caracteres ou a nos vazios,
//e cria uma nova arvore, cujos filhos da raiz sao os nos passados como argumento
HuffmanNode *mergeNodes(HuffmanNode *left, HuffmanNode *right) {
	int sum = right->weight + left->weight; 
	HuffmanNode *newNode;
	newNode = newHuffmanNode(FALSE, 0, sum);
	newNode->l = left;
	newNode->r = right;
	return newNode;
}

//aloca espaco e cria um novo HuffmanNode
HuffmanNode *newHuffmanNode(bool isChar, char character, int weight) {
	HuffmanNode *newNode;
	newNode = (HuffmanNode*)malloc(sizeof(HuffmanNode));
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

QueueNode *newQNode(HuffmanNode *node) {
	QueueNode *newNode = (QueueNode*)malloc(sizeof(QueueNode));
	if(newNode == NULL) {
		fprintf(stderr, "Impossivel alocar espaco. Abortando");
		exit(2);
	}
	newNode->node = node;
	newNode->next = NULL;
	return newNode;
}

void swap(HuffmanNode *nodes, int pos1, int pos2) {
	HuffmanNode aux = nodes[pos1];
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

void printTree(HuffmanNode *root) {
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

//essa funcao retira os dois caracteres de menor peso de uma fila, cria uma subarvore
//e reinsere a subarvore na fila
Queue createSubTree(Queue queue, bool *cont) {
	HuffmanNode *first, *second, *merged;
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
	return q;
}

//essa funcao recebe a saida da funcao parse (um vetor com os caracteres encontrados,
//um vetor com a contagem de ocorrenceias de cada caracter indexavel pelo proprio caracter
//e o numero de caracteres enconjtrados, e monta uma "fila" onde os menores caracteres vem primeiro
Queue buildQueue(char *pChars, int *cCount, int nChars) {
	int i;
	HuffmanNode *currNode;
	Queue q = newQueue();
	for(i = 0; i < nChars; i++) {
		currNode = newHuffmanNode(TRUE, pChars[i], cCount[INT(pChars[i])]);
		q = enq(currNode, q);
	}
	return q;
}

Queue newQueue() {
	return NULL;
}

Queue deq(Queue queue, HuffmanNode **node) {
	*node = queue->node;
	QueueNode *aux = queue->next;
	free(queue);
	return aux;
}

//essa funcao ibtem a altura de uma arvore de HuffmanNode ligada dinamicamente
int height(HuffmanNode *tree) {
	int maxHeight, lHeight, rHeight;
	if (tree == NULL) return 0;
	lHeight = height(tree->l);
	rHeight = height(tree->r);
	maxHeight = lHeight > rHeight ? lHeight : rHeight;
	return 1  + maxHeight;
}

//essa funcao percorre um arquivo txt e registra nos arrays recebidos como argumento
//quais caracteres estao presentes e o numero de ocorrencias de cada caracter
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

//essa funcao insere um no na posicao correta de uma fila ordenada pelo peso do no
Queue enq(HuffmanNode *newNode, Queue queue) {
	QueueNode *new;
	if (queue == NULL) { 
		new = newQNode(newNode);
		return new;
	}
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

void printArrTree(HuffmanNode tree[], int pos) {
	printf("caracter %c: %d ocorrencias\n", tree[pos].c, tree[pos].weight);
	if(tree[pos].isChar) return;	
	if(tree->l != NULL) printArrTree(tree, LC(pos));
	if(tree->r != NULL) printArrTree(tree, RC(pos));
}

//"escreve" um bit para um arquivo binario. como so podemos escrever 8 bits por vez,
//a funcao armazena os bits em um buffer antes de escreve-los. Caso seja necessario
//escrever um conjunto de bits "direto", antes de que 8 bits tenham sido escritos,
//podemos usar setar o argumento forceWrite como TRUE.
void writeBit(int bit, FILE *file, bool forceWrite) {
	static int nWrittenBits = 0;
	static unsigned char buffer = 0x00;
	unsigned char auxByte = 0x00;
	bool shouldWrite = (nWrittenBits == 7) || forceWrite;

	auxByte = auxByte | (bit ? 0x80 : 0x00);
	auxByte = auxByte >> nWrittenBits;
	buffer = buffer | auxByte;
	nWrittenBits++;

	if(shouldWrite) {
		fwrite(&buffer, sizeof(char), 1, file);
		nWrittenBits = 0;
		buffer = 0x00;
	}
}	

//essa funcao le um unico bit de um arquivo binario
int readBit(FILE *file) {
	static int nReadBits = 0;
	static unsigned char buffer;
	int bit;
	if (nReadBits == 0) fread(&buffer, sizeof(unsigned char), 1, file);
	bit = ((buffer & 0x80) == 0x80 ? 1 : 0);
	buffer = buffer<<1;
	nReadBits = (nReadBits + 1) % 8;
	return bit;
}

//monta uma tabela de simbolos em um array de BitPattern de tamanho suficiente (256)
//no qual o indice eh o caracter desejado.
void makeBitPatternTable(HuffmanNode *huffmanTree, BitPattern *table, int pattern, int bitCount) {
	BitPattern bitPattern;
	int lPattern, rPattern;
	if(huffmanTree->isChar) {
		bitPattern.bits = pattern;
		bitPattern.bitCount = bitCount;
		table[INT(huffmanTree->c)] = bitPattern; 
		return;
	}
	lPattern = pattern << 1;
	rPattern = (pattern << 1) | 1;
	makeBitPatternTable(huffmanTree->l, table, lPattern, bitCount+1);
	makeBitPatternTable(huffmanTree->r, table, rPattern, bitCount+1);
	return;
}

void encode(BitPattern *table, FILE *file) {
	char c;
	int i;
	BitPattern bitPattern;
	while(fscanf(file, "%c", &c) == 1) {
		bitPattern = table[INT(c)];
		i = bitPattern.bitCount;
	}
}

void writeBitPattern(char c, BitPattern *table, FILE *file, bool isLastChar) {
	BitPattern pattern = table[INT(c)];
	int bits, currBit, bitCount;
	bool forceWrite;
	bitCount = pattern.bitCount;
	bits = pattern.bits << ((8 * sizeof(int)) - bitCount);
	for(int i = 0; i < pattern.bitCount; i++) {
		forceWrite = isLastChar && (bitCount == 1);
		currBit = (bits & 0x80000000) == 0x80000000 ? 1 : 0;
		writeBit(currBit, file, forceWrite);
		bitCount--;
		bits = bits << 1;
	}
}	

void translateSingleCharacter(FILE *binaryFile, FILE *textFile, HuffmanNode *tree) {
	int bit, pos;
	char c;
	HuffmanNode node;
	pos = 0;
	node = tree[0];
	while(!node.isChar) {
		bit = readBit(binaryFile);
		pos = bit ? RC(pos) : LC(pos);
		node = tree[pos];
	}
	c = node.c;
	fprintf(textFile, "%c", c);
	return;
}
