#include <stdbool.h>

/*
 * Interface for heap.c
 * heap.c implements a priority queue by way of a max heap
 * Implementation includes a function to increase the key of a particular item
 * HeapInsert, HeapIncreaseKey and HeapExtract are log(n) operations
 * Heap is automatically resized when too big or too small
 * Control resizing behavior through RESIZE_FACTOR in heap.c
 */

#ifndef _heap_h
#define _heap_h

struct HeapNode {
    void *data;         // pointer to data object
    struct Heap *heap;  // heap this HeapNode belongs to
    float key;          // node's priority
    int offset;         // node's position in heap (its offset)
};

struct Heap;

struct Heap *HeapNew();

void HeapDestroy(struct Heap *heap);

struct HeapNode *HeapExtract(struct Heap *heap);

bool HeapIncreaseKey(struct HeapNode *node, float key);
// Use HeapNode reference returned by HeapInsert()

struct HeapNode *HeapInsert(struct Heap *heap, void *data, float key);
/*
* When a node is inserted, function returns a HeapNode object
* HeapNode is used as a reference for HeapIncreaseKey
* Discard return value if this functionality isn't needed
*/

bool HeapIsEmpty(struct Heap *heap);

#endif
