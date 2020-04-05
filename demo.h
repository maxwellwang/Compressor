#ifndef DEMO_H
#define DEMO_H

#define ISWHITESPACE(c) (c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\v' || c == '\f')
#define TABLE_INIT_SIZE 10
#define LOAD_FACTOR 0.5



typedef struct _node {
	char* token; // word
	int tokenLength;
	int frequency; // frequency of word or of tree
	struct _node* left; // only applicable for tree
	struct _node* right; // only applicable for tree
} Node;

// A min heap to help build Huffman tree from smallest frequencies up to larger ones
typedef struct _heap {
	Node** heap; // array representing the heap
	int finalIndex; // index of final node in heap
	int size;
} Heap;

//Linked list node
typedef struct llnode {
  struct llnode * next;
  char * name;
} l_node;

void recurse(char * dirname, l_node * head);

// Makes token node from name and frequency
Node* makeTokenNode(char*, int, int);

// Sifts up final node of heap to correct position
void siftUp(Heap*);

// Inserts node into heap
void insertNode(Heap*, Node*);

// Gets number of children of node in heap
int numChildren(Heap*, int, int);

// Sifts down final node of heap to correct position
void siftDown(Heap*);

// Extracts minimum node from heap
Node* extractNode(Heap*);

// Builds tree from 2 nodes, a helper for buildHuffmanTree
Node* buildTree(Node*, Node*);

// Builds Huffman tree from heap
Node* buildHuffmanTree(Heap*);

// Recursively free node
void freeNode(Node*);

// Frees all nodes in min heap
void freeHeap(Heap*);

#endif
