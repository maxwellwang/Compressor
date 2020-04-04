#include <stdio.h>
#include <stdlib.h>
#include "demo.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

void recurse(char * dirname) {
  struct dirent *dp;
  DIR * dir_struct = opendir(dirname);
  char * name;
  char * pathname;
  char type;

  if (!dir_struct) {
    printf("Bye %s %s\n", dirname, strerror(errno));
    return;
  }
  
  while ((dp = readdir(dir_struct)) != NULL) { //skipping opening files
    name = dp->d_name;
    type = dp->d_type;
    if (type == DT_DIR) {
      if ((strcmp(name,".") * strcmp(name,".."))) {
	//	printf("Directory found: %s\n", name);
	pathname = (char*)malloc(strlen(dirname) + strlen(name) + 1 + 1);
	if (!pathname) {
		printf("Error: Malloc failed\n");
		exit(EXIT_FAILURE);
	}
	memset(pathname, 0, strlen(dirname) + strlen(name) + 1 + 1);
	memcpy(pathname, dirname, strlen(dirname));
	pathname[strlen(pathname)] = '/';
	memcpy(pathname + strlen(dirname)+1, name, strlen(name));
	recurse(pathname);
	free(pathname);
      }
    } else if (type == DT_REG) {
      printf("File found: %s\n", name);
	     //this is for printout out file content
	     // ONLY uncomment if there's only normal text files, or else will print out a mess	     
//      FILE * fd = open(name, O_RDONLY);
//      char c;
//      while (read(fd, &c, 1)) {
//	printf("%c", c);
//      }
//      printf("\n");
    }
  }
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
	temp->tokenLength = tokenLength;
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
	if (aHeap->finalIndex == aHeap->size) {
		// need to realloc
		Node** temp = (Node**)malloc(sizeof(Node*) * aHeap->size);
		memcpy(temp, aHeap->heap, aHeap->size);
		aHeap->size *= 2;
		free(aHeap->heap);
		aHeap->heap = temp;
	}
	(aHeap->heap)[aHeap->finalIndex] = aNode;
	siftUp(aHeap);
	(aHeap->size)++;
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
	(aHeap->size)--;
	return ret;
}

// Builds tree from 2 nodes
Node* buildTree(Node* nodeA, Node* nodeB) {
	if (!nodeB) {
		// only one token in file, return it
		return nodeA;
	}
	Node* temp = (Node*)malloc(sizeof(Node));
	if (!temp) {
		printf("Error: Malloc failed\n");
		exit(EXIT_FAILURE);
	}
	temp->token = NULL;
	temp->tokenLength = 0;
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
	free(aHeap->heap);
	return;
}
