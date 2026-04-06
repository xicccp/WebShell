#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdio.h>

#define HEAP_SIZE 1024*1024 // 1MB
#define NUM_BINS 10

#define ALIGN8(x) (((x) + 7) & ~7)
#define MIN_BLOCK_SIZE 32

#define HEADER_SIZE sizeof(Block)
#define FOOTER_SIZE sizeof(Footer)

typedef struct Block {
    size_t size;
    struct Block* next;
    struct Block* prev;
    int free;
} Block;

typedef struct Footer {
    size_t size;
} Footer;

// Heap memory
extern char heap[HEAP_SIZE];

// Free bins
extern Block* free_bins[NUM_BINS];

// Functions
void init_heap();
void* my_malloc(size_t size);
void my_free(void* ptr);
void* my_realloc(void* ptr, size_t size);
void write_footer(Block* block);
Block* next_phys(Block* block);
Block* prev_phys(Block* block);
int get_bin(size_t size);
void insert_block(Block* block);
void remove_block(Block* block);

// Debug helpers
void print_heap();

#endif // ALLOCATOR_H
