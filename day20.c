/*
1. Implement fast ﬁle copy program (assume max ﬁle size = 1 GB).
step 1: open src ﬁle in rdonly mode.
step 2: get size of src ﬁle (fstat() syscall)
step 3: map src ﬁle contents to memory using mmap()
step 4: create dest ﬁle in rdwr mode.
step 5: make size of dest ﬁle, same as size of src ﬁle using ftruncate()
step 6: map dest ﬁle contents to memory using mmap() -- MAP_SHARED.
step 7: copy src ﬁle to dest ﬁle using memcpy()
step 8: close src and dest ﬁles.*/
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

int main() {
	struct stat st;
	int fd, ret;
	void *ptr;
	fd = open("/home/om/Desktop/test.c", O_RDONLY);
	if(fd < 0) {
		perror("open() failed");
		_exit(1);
	}
	ret = fstat(fd, &st);
	if(ret < 0) {
		perror("fstat() failed");
		close(fd);
		_exit(2);
	}
	printf("reading file of size: %lu\n", st.st_size);

	ptr = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if(ptr == (void*)-1) {
		perror("mmap() failed");
		close(fd);
		_exit(3);
	}

	// stdout=1
	write(1, ptr, st.st_size);

	close(fd);
	return 0;
}

