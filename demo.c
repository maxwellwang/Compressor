#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "demo.h"

int h_size = TABLE_INIT_SIZE;
int h_items = 0;

//borrowed from Dan Bernstein
int h_func(char * str) {
  unsigned long hash = 5381;
  int c;
  while (c = *str++) {
    hash = ((hash << 5) + hash) + c;
  }
  return hash % h_size;
}


h_node * h_init() {
  h_node* p = (h_node*)malloc(h_size * sizeof(h_node));
  if (!p) {
  	printf("Error: Malloc failed\n");
  	exit(EXIT_FAILURE);
  }
  int i = 0;
  for (; i < h_size; i++) {
    (p[i]).string = NULL;
    (p[i]).hashcode = 0;
    (p[i]).freq = 0;
  }
  return p;  
}

h_node * h_rehash(h_node * h_table);

//inserts string if doesn't exist, adds count if it does
//returns pointer to hash table
h_node * h_add(h_node * h_table, char * str, int count) {
  float lf = LOAD_FACTOR;
  if ((float) h_items / (float) h_size >= lf) {
    h_table = h_rehash(h_table);
  }  
  int hash_code = h_func(str);
  int i = 0, index = hash_code % h_size;
  while (h_table[index].string != NULL && strcmp(h_table[index].string, str) != 0) { 
    index = (hash_code + i++) % h_size;
  }
  (h_table[index]).string = str; //should I instead malloc something new? or nah
  (h_table[index]).hashcode = hash_code;
  (h_table[index]).freq += count;
  h_items += h_table[index].freq == count ? 1 : 0;
  return h_table;
}


h_node * h_rehash(h_node * h_table) {
  h_size *= 2;
  h_items = 0;
  h_node * h_tmp = h_init();
  int i = 0;
  for (; i < h_size; i++) {
    (h_tmp[i]).string = NULL;
    (h_tmp[i]).hashcode = 0;
    (h_tmp[i]).freq = 0;
  }
  for (i = 0; i < h_size/2; i++) {
    if ((h_table)[i].string != NULL) {
      h_tmp = h_add(h_tmp, (h_table)[i].string, (h_table)[i].freq);
    }
  }
  free(h_table);
  return h_tmp;
}


// Makes token node from name and frequency
Node* makeTokenNode(char* token, int tokenLength, int frequency) {
	Node* temp = (Node*)malloc(sizeof(Node));
	if (!temp) {
		printf("Error: Malloc failed\n");
		exit(EXIT_FAILURE);
	}
	temp->token = (char*)malloc(tokenLength);
	if (!(temp->token)) {
		printf("Error: Malloc failed\n");
		exit(EXIT_FAILURE);
	}
	strcpy(temp->token, token);
	temp->frequency = frequency;
	temp->left = NULL;
	temp->right = NULL;
	return temp;
}

// Sifts up final node of heap to correct position
void siftUp(Heap* aHeap) {
	int c = aHeap->finalIndex;
	int p = (c - 1) / 2;
	if (c != 0) {
		Node* child = (aHeap->heap)[c];
		Node* parent = (aHeap->heap)[p];
		// If child has lower frequency than parent, then swap
		while (c != 0 && child->frequency < parent->frequency) {
			(aHeap->heap)[c] = parent;
			(aHeap->heap)[p] = child;
			c = p;
			p = (c - 1) / 2;
			child = (aHeap->heap)[c];
			parent = (aHeap->heap)[p];
		}
	}
	return;
}

// Inserts node into heap
void insertNode(Heap* aHeap, Node* aNode) {
	(aHeap->finalIndex)++;
	(aHeap->heap)[aHeap->finalIndex] = aNode;
	siftUp(aHeap);
	return;
}

// Gets number of children of node in heap
int numChildren(Heap* aHeap, int c0, int c1) {
	int ret = 0;
	if (c0 <= aHeap->finalIndex) {
		ret++;
	}
	if (c1 <= aHeap->finalIndex) {
		ret++;
	}
	return ret;
}

// Sifts down final node of heap to correct position
void siftDown(Heap* aHeap) {
	int p = 0;
	int c0 = 2 * p + 1;
	int c1 = c0 + 1;
	if (numChildren(aHeap, c0, c1) > 0) {
		Node* parent = (aHeap->heap)[p];
		Node* child0 = (aHeap->heap)[c0];
		Node* child1 = NULL;
		if (numChildren(aHeap, c0, c1) == 2) {
			child1 = (aHeap->heap)[c1];
		}
		// If parent has higher frequency than one of its children, swap with lower frequency child
		while (numChildren(aHeap, c0, c1) > 0 && (parent->frequency > child0->frequency || (child1 && parent->frequency > child1->frequency))) {
			if (child1 && child1->frequency < child0->frequency) {
				// Swap child1 with parent
				(aHeap->heap)[p] = child1;
				(aHeap->heap)[c1] = parent;
				p = c1;
				parent = (aHeap->heap)[p];
			} else {
				// Swap child0 with parent
				(aHeap->heap)[p] = child0;
				(aHeap->heap)[c0] = parent;
				p = c0;
				parent = (aHeap->heap)[p];
			}
			c0 = 2 * p + 1;
			c1 = c0 + 1;
			child0 = (aHeap->heap)[c0];
			child1 = (aHeap->heap)[c1];
		}
	}
	return;
}

// Extracts minimum node from heap
Node* extractNode(Heap* aHeap) {
	Node* ret = (aHeap->heap)[0];
	if (!ret) {
		return NULL;
	}
	(aHeap->heap)[0] = (aHeap->heap)[aHeap->finalIndex];
	(aHeap->heap)[aHeap->finalIndex] = NULL;
	(aHeap->finalIndex)--;
	siftDown(aHeap);
	return ret;
}

// Builds tree from 2 nodes
Node* buildTree(Node* nodeA, Node* nodeB) {
	Node* temp = (Node*)malloc(sizeof(Node));
	if (!temp) {
		printf("Error: Malloc failed\n");
		exit(EXIT_FAILURE);
	}
	temp->token = NULL;
	temp->frequency = nodeA->frequency + nodeB->frequency;
	temp->left = nodeA;
	temp->right = nodeB;
	return temp;
}

// Builds Huffman tree from heap
Node* buildHuffmanTree(Heap* aHeap) {
	if (!aHeap) {
		return NULL;
	}
	Node* nodeA = NULL;
	Node* nodeB = NULL;
	Node* temp = NULL;
	// Build trees until you have one left
	while (aHeap->finalIndex > 0) {
		nodeA = extractNode(aHeap);
		nodeB = extractNode(aHeap);
		temp = buildTree(nodeA, nodeB);
		insertNode(aHeap, temp);
	}
	return (aHeap->heap)[0];
}

// Recursively free node
void freeNode(Node* aNode) {
	if (!aNode) {
		return;
	}
	freeNode(aNode->left);
	free(aNode->token);
	free(aNode);
	freeNode(aNode->right);
	return;
}

// Frees all nodes in min heap
void freeHeap(Heap* aHeap) {
	Node* currNode = extractNode(aHeap);
	if (!currNode) {
		return;
	}
	while (currNode) {
		freeNode(currNode);
		currNode = extractNode(aHeap);
	}
	return;
}
