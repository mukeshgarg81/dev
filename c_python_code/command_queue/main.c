#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#define COMMAND_QUEUE_CAPACITY 10

typedef struct {
    int commandType;
    int data;
} Command;

typedef struct {
    Command commands[COMMAND_QUEUE_CAPACITY];
    int front;
    int rear;
    pthread_mutex_t mutex;
} CommandQueue;

CommandQueue g_commandQueue;

// Initialize the command queue
void init_command_queue() {
    g_commandQueue.front = 0;
    g_commandQueue.rear = 0;
    pthread_mutex_init(&g_commandQueue.mutex, NULL);
}

// Enqueue a command into the command queue
void enqueue_command(int commandType, int data) {
    pthread_mutex_lock(&g_commandQueue.mutex);
    
    int nextRear = (g_commandQueue.rear + 1) % COMMAND_QUEUE_CAPACITY;
    if (nextRear != g_commandQueue.front) {
        g_commandQueue.commands[g_commandQueue.rear].commandType = commandType;
        g_commandQueue.commands[g_commandQueue.rear].data = data;
        g_commandQueue.rear = nextRear;
    } else {
        printf("Command queue is full, cannot enqueue command.\n");
    }

    pthread_mutex_unlock(&g_commandQueue.mutex);
}

// Dequeue a command from the command queue
bool dequeue_command(Command *command) {
    pthread_mutex_lock(&g_commandQueue.mutex);
    
    if (g_commandQueue.front != g_commandQueue.rear) {
        *command = g_commandQueue.commands[g_commandQueue.front];
        g_commandQueue.front = (g_commandQueue.front + 1) % COMMAND_QUEUE_CAPACITY;
        pthread_mutex_unlock(&g_commandQueue.mutex);
        return true;
    } else {
        pthread_mutex_unlock(&g_commandQueue.mutex);
        return false;
    }
}

// Execute a command
void execute_command(Command command) {
    switch (command.commandType) {
        case 1:
            printf("Executing Command 1 with data: %d\n", command.data);
            // Add code for command 1 execution
            break;
        case 2:
            printf("Executing Command 2 with data: %d\n", command.data);
            // Add code for command 2 execution
            break;
        // Add more command cases as needed
        default:
            printf("Unknown command type: %d\n", command.commandType);
            break;
    }
}

int main() {
    init_command_queue();

    // Simulate receiving commands from host and enqueueing them
    enqueue_command(1, 42);
    enqueue_command(2, 10);
    enqueue_command(3, 5);

    // Process and execute commands
    Command command;
    while (dequeue_command(&command)) {
        execute_command(command);
    }

    return 0;
}


