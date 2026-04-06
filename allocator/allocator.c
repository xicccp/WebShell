#include "allocator.h"
#include <string.h>

// Heap memory
char heap[HEAP_SIZE];
Block* free_list;

void init_heap() {
    free_list = (Block*)heap;
    free_list->size = HEAP_SIZE - sizeof(Block);
    free_list->next = NULL;
    free_list->free = 1;
}

void* my_malloc(size_t size) {
    if (size <= 0) {
        return NULL;
    }
    Block* current = free_list;
    while(current) {
        if (current->free && current->size >= size) {
            // Found suitable block to split
            if (current->size > size + sizeof(Block)) {
                Block* new_block = (Block*)((char*)current + sizeof(Block) + size);
                new_block->size = current->size - size - sizeof(Block);
                new_block->next = current->next;
                new_block->free = 1;
                current->size = size;
                current->next = new_block;
            }
            current->free = 0;
            return (char*) current + sizeof(Block);
        }
        current = current->next;
    }
    return NULL; // No suitable block found
}

void my_free(void* ptr) {
    if (ptr == NULL) {
        return;
    }
    Block* header = (Block*)((char*)ptr - sizeof(Block));
    header->free = 1;
    if(header->next && header->next->free) {
        header->size += sizeof(Block) + header->next->size;
        header->next = header->next->next;
    }
}

size_t min (size_t a, size_t b) {
    return (a < b) ? a : b;
}

void* my_realloc(void* ptr, size_t size) {
    if (ptr == NULL)
        return my_malloc(size);
    if (size == 0) {
        my_free(ptr);
        return NULL;
    }
    Block* header = (Block*)((char*)ptr - sizeof(Block));
    if (header->size >= size) {
        return ptr; // Current block is enough
    }

    void* new_ptr = my_malloc(size);
    if(new_ptr == NULL) {
        return NULL; //Allocation failed
    }
    memcpy(new_ptr, ptr, min(header->size, size));
    my_free(ptr);
    return new_ptr;
 }

void print_heap() {
    Block* current = (Block*)heap;
    int i = 1;
    while (current < (Block*)(heap + HEAP_SIZE)) {
        printf("Block %d at %p: size=%zu, free=%d\n", i, (void*)current, current->size, current->free);
        current = (Block*)((char*)current + sizeof(Block) + current->size);
        i++;
    }
}
