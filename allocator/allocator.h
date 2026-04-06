#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdio.h>

#define HEAP_SIZE 1024*1024 // 1MB

typedef struct Block {
    size_t size;
    struct Block* next;
    int free;
} Block;

// Heap memory
extern char heap[HEAP_SIZE];

// Free list head 
extern Block* free_list;

// Functions
void init_heap();
void* my_malloc(size_t size);
void my_free(void* ptr);
void* my_realloc(void* ptr, size_t size);

// Debug helpers
void print_heap();

#endif // ALLOCATOR_H
