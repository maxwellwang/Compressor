#ifndef _HUFFMAN_DATA_STRUCTURES_H
#define _HUFFMAN_DATA_STRUCTURES_H

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
Node* makeTokenNode(char* token, int frequency);

// Makes heap from text file
Heap* makeHeap(char* path);

// Sifts up final node in heap to correct position
void siftUp(Heap* aHeap);

// Inserts node into heap
void insertNode(Heap* aHeap, Node* aNode);

// Sifts down first node in heap to correct position
void siftDown(Heap* aHeap);

// Extracts minimum node from heap
Node* extractNode(Heap* aHeap);

// Builds tree from 2 nodes
Node* buildTree(Node* nodeA, Node* nodeB);

// Builds Huffman tree from min heap
Node* buildHuffmanTree(Heap* aHeap);

// Builds codebook from Huffman tree
void buildCodebook(Node* aNode);

#endif
