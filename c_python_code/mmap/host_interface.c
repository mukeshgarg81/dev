// example.c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>

int* create_and_map_file(const char* filename, int size) {
    int fd = open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    if (lseek(fd, size * sizeof(int) - 1, SEEK_SET) == -1) {
        perror("lseek");
        close(fd);
        exit(1);
    }

    if (write(fd, "", 1) == -1) {
        perror("write");
        close(fd);
        exit(1);
    }

    int* arr = (int*)mmap(NULL, size * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (arr == MAP_FAILED) {
	            perror("mmap");
        close(fd);
        exit(1);
    }

    close(fd);
    return arr;
}

void unmap_and_close(int* arr, int size) {
    if (munmap(arr, size * sizeof(int)) == -1) {
        perror("munmap");
        exit(1);
    }
}

