#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source file> <destination file>\n", argv[0]);
        return 1;
    }

    const char *src_path = argv[1];
    const char *dest_path = argv[2];

    struct stat st;
    int src_fd, dest_fd;
    void *src_ptr, *dest_ptr;

    // Step 1: Open source file in read-only mode
    src_fd = open(src_path, O_RDONLY);
    if (src_fd < 0) {
        perror("open(src) failed");
        return 1;
    }

    // Step 2: Get size of source file
    if (fstat(src_fd, &st) < 0) {
        perror("fstat() failed");
        close(src_fd);
        return 2;
    }

    // Step 3: Map source file contents to memory
    src_ptr = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, src_fd, 0);
    if (src_ptr == MAP_FAILED) {
        perror("mmap(src) failed");
        close(src_fd);
        return 3;
    }

    // Step 4: Create destination file in read-write mode
    dest_fd = open(dest_path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (dest_fd < 0) {
        perror("open(dest) failed");
        munmap(src_ptr, st.st_size);
        close(src_fd);
        return 4;
    }

    // Step 5: Make size of destination file same as source file
    if (ftruncate(dest_fd, st.st_size) < 0) {
        perror("ftruncate() failed");
        munmap(src_ptr, st.st_size);
        close(src_fd);
        close(dest_fd);
        return 5;
    }

    // Step 6: Map destination file contents to memory
    dest_ptr = mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, dest_fd, 0);
    if (dest_ptr == MAP_FAILED) {
        perror("mmap(dest) failed");
        munmap(src_ptr, st.st_size);
        close(src_fd);
        close(dest_fd);
        return 6;
    }

    // Step 7: Copy source file to destination file using memcpy
    memcpy(dest_ptr, src_ptr, st.st_size);

    // Step 8: Close source and destination files
    munmap(src_ptr, st.st_size);
    munmap(dest_ptr, st.st_size);
    close(src_fd);
    close(dest_fd);

    return 0;
}
