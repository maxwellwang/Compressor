#ifndef HUFFMAN_DATA_STRUCTURES_H
#define HUFFMAN_DATA_STRUCTURES_H

typedef struct Node {
	char* name;
	int frequency;
	struct Node* left;
	struct Node* right;
} Node;

Node extractNode(Node heap[]);

void insertNode(Node heap[], Node aNode);

#endif
