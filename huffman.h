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

// Typedef for dirent structure
typedef struct dirent Dirent;

// Makes token node from name and frequency
Node* makeTokenNode(char*, int);

// Inserts node into min heap
void insertNode(Heap*, Node*);

// Extracts minimum node from min heap
Node* extractNode(Heap*);

// Builds Huffman tree from min heap
Node* buildHuffmanTree(Heap*);

#endif
