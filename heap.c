#include "heap.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>
#include <string.h>

#define RESIZE_FACTOR   2                   // Controls how often and how aggressively heap auto-resizes
#define MIN_SIZE        10                  // Must be greater than 1. For efficiency the 0th node is ignored.
                                            // Making this less than 2 will break the code.
#define PARENT(i)       ((i) >> 1)
#define LEFT(i)         ((i) << 1)
#define RIGHT(i)        (((i) << 1) | 1)

struct Heap {
    struct HeapNode **nodes;    // pointer to array of HeapNodes
    int size;                   // current size of heap
    int max_size;               // maximum size of heap
};

struct Heap *HeapNew() {
    struct Heap *new_heap;
    struct HeapNode **new_nodes;
    new_heap = (struct Heap *)malloc(sizeof(struct Heap));
    new_nodes = (struct HeapNode **)malloc(MIN_SIZE * sizeof(struct HeapNode *));
    if (new_heap && new_nodes) {
        new_heap->nodes = new_nodes;
        new_heap->size = 0;
        new_heap->max_size = MIN_SIZE;
        return new_heap;
    }
    else
        return NULL;
}

void HeapDestroy(struct Heap *heap) {
    int i;
    if (!heap || !heap->nodes)
        return;
    for (i = 1; i < heap->size; i++)
        free(heap->nodes[i]);
    free(heap->nodes);
    free(heap);
}

static bool resize(struct Heap *heap) {
    struct HeapNode **resized_nodes;        // Array of HeapNodes is the internal representation of heap in struct Heap
    // notice how we resize from heap->size, not heap->max_size. In this way, the operation can both shrink and enlarge.
    resized_nodes = (struct HeapNode **)realloc(heap->nodes, heap->size * RESIZE_FACTOR * sizeof(struct HeapNode *));
    if (resized_nodes == NULL) {
        return false;
    }
    else {
        heap->nodes = resized_nodes;
        heap->max_size = heap->size * RESIZE_FACTOR;
        return true;
    }
}

static void heapify(struct Heap *heap, struct HeapNode *node) {
    int i, j, left, right;
    struct HeapNode *swap;
    i = node->offset;
    j = i;
    for(;;) {
        left = LEFT(i); right = RIGHT(i);
        if (left <= heap->size && node->key < heap->nodes[left]->key)
            j = left;
        if(right <= heap->size && heap->nodes[j]->key < heap->nodes[right]->key)
            j = right;
        if (i != j) {
            swap = node;
            heap->nodes[i] = heap->nodes[j];
            heap->nodes[j] = node;
            heap->nodes[i]->offset = i;
            heap->nodes[j]->offset = j;
            i = j;
        }
        else
            break;
    }
}

struct HeapNode *HeapExtract(struct Heap *heap) {
    struct HeapNode *ret;
    if (heap->size < 1) {
        return NULL;
    }
    else {
        ret = heap->nodes[1];
        heap->nodes[1] = heap->nodes[heap->size];
        heap->nodes[1]->offset = 1;
        heap->size--;
        heapify(heap, heap->nodes[1]);
    }
    if (heap->size < heap->max_size/(RESIZE_FACTOR*RESIZE_FACTOR) && heap->size > MIN_SIZE)
        resize(heap);   // This operation may fail, but it should not affect the HeapExtract() call
    return ret;
}

bool HeapIncreaseKey(struct HeapNode *node, float key) {
    // What it says on the tin. Increases (and ONLY increases) the key to a given node
    int i, j;
    struct Heap *heap = node->heap;

    if (key < node->key){
        return false;
    }
    else {
        node->key = key;
        i = node->offset;
        j = PARENT(i);
        while (i > 1 && heap->nodes[j]->key < key){
            heap->nodes[i] = heap->nodes[j];
            heap->nodes[j] = node;
            heap->nodes[i]->offset = i;
            heap->nodes[j]->offset = j;
            i = j;
            j = PARENT(i);
        }
        return true;
    }
}

struct HeapNode *HeapInsert(struct Heap *heap, void *data, float key) {
    struct HeapNode *new_node;
    new_node = (struct HeapNode *)malloc(sizeof(struct HeapNode));
    if (new_node == NULL || (heap->size == heap->max_size - 1 && !resize(heap))) {
        return NULL;    // failed to allocate extra memory
    }
    else {
        heap->size++;
        heap->nodes[heap->size] = new_node;
        new_node->offset=heap->size;
        new_node->data = data;
        new_node->heap = heap;
        new_node->key = -FLT_MAX;
        HeapIncreaseKey(new_node, key);
        return new_node;
    }
}

bool HeapIsEmpty(struct Heap *heap) {
    if (heap->size > 0)
        return false;
    else
        return true;
}




