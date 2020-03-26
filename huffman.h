#ifndef HUFFMAN_H
#define HUFFMAN_H

// Can represent either a word with frequency or a tree of words
typedef struct _node {
	char* token; // word
	int frequency; // frequency of word or of tree
	struct _node* left; // only applicable for tree
	struct _node* right; // only applicable for tree
} Node;

// A min heap to help build Huffman tree from smallest frequencies up to larger ones
typedef struct _heap {
	Node** heap; // array representing the heap
	int finalIndex; // index of final node in heap
} Heap;

// Makes token node
Node* makeTokenNode(char*, int);

// Sifts up final node in heap to correct position
void siftUp(Heap*);

// Inserts node into heap
void insertNode(Heap*, Node*);

// Sifts down first node in heap to correct position
void siftDown(Heap*);

// Extracts minimum node from heap
Node* extractNode(Heap*);

// Builds tree from 2 nodes
Node* buildTree(Node*, Node*);

// Builds Huffman tree from min heap
Node* buildHuffmanTree(Heap*);

// Builds codebook from Huffman tree
void buildCodebook(Node*);

#endif
