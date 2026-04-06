#include "allocator.h"
#include <string.h>

// Heap memory
char heap[HEAP_SIZE];
Block* free_bins[NUM_BINS];

void write_footer(Block* b) {
    Footer* f = (Footer*)((char*)b + HEADER_SIZE + b->size);
    f->size = b->size;
}

Block* next_phys(Block* b) {
    Block* next = (Block*)((char*)b + HEADER_SIZE + b->size + FOOTER_SIZE);
    if ((char*)next >= heap + HEAP_SIZE) return NULL;
    return next;
}

Block* prev_phys(Block* b) {
    if ((char*)b == heap) return NULL;

    Footer* f = (Footer*)((char*)b - FOOTER_SIZE);
    Block* prev = (Block*)((char*)b - HEADER_SIZE - f->size - FOOTER_SIZE);

    if ((char*)prev < heap) return NULL;
    return prev;
}

int get_bin(size_t size) {
    if (size <= 32) return 0;

    int bin = 0;
    size = (size - 1) >> 5;

    while (size > 0) {
        size >>= 1;
        bin++;
    }

    return bin >= NUM_BINS ? NUM_BINS - 1 : bin;
}

void insert_block(Block* b) {
    int bin = get_bin(b->size);
    b->next = free_bins[bin];
    b->prev = NULL;

    if (free_bins[bin])
        free_bins[bin]->prev = b;

    free_bins[bin] = b;
}

void remove_block(Block* b) {
    int bin = get_bin(b->size);

    if (b->prev)
        b->prev->next = b->next;
    else
        free_bins[bin] = b->next;

    if (b->next)
        b->next->prev = b->prev;
}

void init_heap() {
    for (int i = 0; i < NUM_BINS; i++)
        free_bins[i] = NULL;

    Block* initial = (Block*)heap;
    initial->size = HEAP_SIZE - HEADER_SIZE - FOOTER_SIZE;
    initial->free = 1;
    initial->next = initial->prev = NULL;

    write_footer(initial);
    insert_block(initial);
}

void* my_malloc(size_t size) {
    if (size == 0) return NULL;

    size = ALIGN8(size);
    int bin = get_bin(size);

    for (int i = bin; i < NUM_BINS; i++) {
        Block* curr = free_bins[i];

        while (curr) {
            if (curr->size >= size) {
                remove_block(curr);

                size_t remaining = curr->size - size;

                if (remaining >= HEADER_SIZE + FOOTER_SIZE + MIN_BLOCK_SIZE) {
                    // split
                    Block* new_block = (Block*)((char*)curr + HEADER_SIZE + size + FOOTER_SIZE);
                    new_block->size = remaining - HEADER_SIZE - FOOTER_SIZE;
                    new_block->free = 1;
                    new_block->next = new_block->prev = NULL;

                    write_footer(new_block);
                    insert_block(new_block);

                    curr->size = size;
                }

                curr->free = 0;
                write_footer(curr);

                return (char*)curr + HEADER_SIZE;
            }
            curr = curr->next;
        }
    }

    return NULL;
}

void my_free(void* ptr) {
    if (!ptr) return;

    Block* b = (Block*)((char*)ptr - HEADER_SIZE);
    b->free = 1;

    Block* prev = prev_phys(b);
    Block* next = next_phys(b);

    // coalesce with prev
    if (prev && prev->free) {
        remove_block(prev);
        prev->size += HEADER_SIZE + FOOTER_SIZE + b->size;
        b = prev;
    }

    // coalesce with next
    if (next && next->free) {
        remove_block(next);
        b->size += HEADER_SIZE + FOOTER_SIZE + next->size;
    }

    write_footer(b);
    insert_block(b);
}

size_t min_size(size_t a, size_t b) {
    return (a < b) ? a : b;
}

void* my_realloc(void* ptr, size_t size) {
    if (!ptr) return my_malloc(size);
    if (size == 0) {
        my_free(ptr);
        return NULL;
    }

    size = ALIGN8(size);

    Block* b = (Block*)((char*)ptr - HEADER_SIZE);

    if (b->size >= size)
        return ptr;

    void* new_ptr = my_malloc(size);
    if (!new_ptr) return NULL;

    memcpy(new_ptr, ptr, b->size);
    my_free(ptr);

    return new_ptr;
}

void print_heap() {
    Block* current = (Block*)heap;
    int i = 1;
    while ((char*)current < heap + HEAP_SIZE) {
        printf("Block %d at %p: size=%zu, free=%d\n", i, (void*)current, current->size, current->free);
        current = (Block*)((char*)current + sizeof(Block) + current->size + sizeof(Footer));
        i++;
    }
}
