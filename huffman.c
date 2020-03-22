#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

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
