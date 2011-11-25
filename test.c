#include "heap.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
	struct Heap *myheap;
	struct HeapNode *node;
	int nums[1000];
	int i;
	int j = 0;
	struct HeapNode *nodes[1000];
	myheap = HeapNew();
	do {
		scanf("%d", &i);
		nums[j] = i;
		nodes[j++] = HeapInsert(myheap, NULL, -17237);
	} while (i > 0);
	if(HeapIsEmpty(myheap))
		printf("Heap is empty.\n");
	else
		printf("Heap is not empty.\n");
	for (i = 0; i < j; i++)
		HeapIncreaseKey(nodes[i], nums[i]); 
	node = HeapExtract(myheap);
	while (node != NULL) {
		printf("%.0f ", node->key);
		node = HeapExtract(myheap);
	}
	if(HeapIsEmpty(myheap))
		printf("\nHeap is empty.\n");
	else
		printf("\nHeap is not empty.\n");
	HeapDestroy(myheap);
}
