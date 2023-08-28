#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

#define MEMORY_SIZE (1024 * 1024)  // 1 MB

typedef struct Block {
    size_t size;
    struct Block *next;
} Block;

Block *head = NULL;

void initialize_memory() {
    head = (Block *)mmap(NULL, MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (head == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    head->size = MEMORY_SIZE - sizeof(Block);
    head->next = NULL;
}

void *mymalloc(size_t size) {
    Block *curr = head;
    Block *prev = NULL;

    while (curr) {
        if (curr->size >= size) {
            if (curr->size >= size + sizeof(Block)) {
                Block *new_block = (Block *)((char *)curr + sizeof(Block) + size);
                new_block->size = curr->size - size - sizeof(Block);
                new_block->next = curr->next;
                curr->size = size;
                curr->next = new_block;
            }

            if (prev) {
                prev->next = curr->next;
            } else {
                head = curr->next;
            }

            return (char *)curr + sizeof(Block);
        }

        prev = curr;
        curr = curr->next;
    }

    return NULL;
}

void myfree(void *ptr) {
    if (!ptr) {
        return;
    }

    Block *block = (Block *)((char *)ptr - sizeof(Block));
    block->next = head;
    head = block;
}

int main() {
    initialize_memory();

    int *arr = (int *)mymalloc(5 * sizeof(int));
    if (arr) {
        for (int i = 0; i < 5; ++i) {
            arr[i] = i * i;
            printf("%d ", arr[i]);
        }
        printf("\n");

        myfree(arr);
    }

    munmap(head, MEMORY_SIZE);

    return 0;
}

