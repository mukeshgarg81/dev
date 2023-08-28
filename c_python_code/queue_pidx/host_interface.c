// example.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>

#define QUEUE_CAPACITY 10

typedef struct {
    int buffer[QUEUE_CAPACITY];
    int pidx;  // Producer index
    int cidx;  // Consumer index
} Queue;

Queue* create_queue(const char* filename) {
    int fd = open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        exit(1);
    }
    if (ftruncate(fd, sizeof(Queue)) == -1) {
        perror("ftruncate");
        close(fd);
        exit(1);
    }

    Queue* queue = (Queue*)mmap(NULL, sizeof(Queue), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (queue == MAP_FAILED) {
        perror("mmap");
        close(fd);
        exit(1);
    }

    close(fd);

    queue->pidx = 0;
    queue->cidx = 0;

    return queue;
}

void enqueue(Queue* queue, int value) {
    int nextPidx = (queue->pidx + 1) % QUEUE_CAPACITY;
    if (nextPidx != queue->cidx) {
        queue->buffer[queue->pidx] = value;
        queue->pidx = nextPidx;
    } else {
        printf("Queue is full, cannot enqueue %d\n", value);
    }
}

int dequeue(Queue* queue) {
    if (queue->cidx != queue->pidx) {
        int value = queue->buffer[queue->cidx];
        queue->cidx = (queue->cidx + 1) % QUEUE_CAPACITY;
        return value;
    } else {
        printf("Queue is empty\n");
        return -1;
    }
}
