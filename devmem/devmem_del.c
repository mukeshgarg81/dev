#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/mman.h>
int main( int argc, char *argv[]) {
	char *mem, tmpMem;
	int fd;
	int i = 0;

	fd = open ("/dev/mem", O_RDWR);
	assert(fd >= 0);
	mem = mmap(NULL, getpagesize(), PROT_READ|PROT_WRITE, MAP_SHARED, fd, (off_t) 0x88101000);
	assert(mem != MAP_FAILED);
	printf("Memory pointer: %p\n", mem);
	printf("The PCI memory is : %#x\n", *mem);
	//*mem = *argv[1];
	mem = mem+0x20;
	tmpMem = mem;
	for(i = 0; i < 15; i++)
	{	printf("%2x ", *mem);
		*mem = 0;
		mem++;
	}
	mem = tmpMem;
	for(i = 0; i < 4; i++)
	{	printf("%2x ", *mem);
		mem++;
	}
	munmap(tmpMem, getpagesize());
	close(fd);
	return 0;
}
