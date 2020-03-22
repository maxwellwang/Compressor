#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

// Sifts up final node of heap to correct position
void siftUp(Heap* aHeap) {
	int c = aHeap->finalIndex;
	int p = (c - 1) / 2;
	Node* child = (aHeap->heap)[c];
	Node* parent = (aHeap->heap)[p];
	// If child has lower frequency than parent, swap
	while (c != 0 && child->frequency < parent->frequency) {
		(aHeap->heap)[c] = parent;
		(aHeap->heap)[p] = child;
		c = p;
		p = (c - 1) / 2;
		child = (aHeap->heap)[c];
		parent = (aHeap->heap)[p];
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

// Sifts down final node of heap to correct position
void siftDown(Heap* aHeap) {
	int p = 0;
	int c0 = 2 * p + 1;
	int c1 = 2 * p + 2;
	Node* parent = (aHeap->heap)[p];
	Node* child0 = (aHeap->heap)[c0];
	Node* child1 = (aHeap->heap)[c1];
	while (c0 > aHeap->finalIndex && c1 > aHeap->finalIndex) {
		
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
