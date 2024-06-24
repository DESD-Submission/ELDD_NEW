#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main() {
	int i, fd;
	char buf[32];
	fd = open("/dev/desd1", O_RDWR);
	if(fd < 0) {
		perror("open() failed");
		_exit(1);
	}

	for(i=1; i<=20; i++) {
		strcpy(buf, "A");
		write(fd, buf, 2);

		memset(buf, 0, sizeof(buf));
		read(fd, buf, sizeof(buf));
		printf("Result: %s\n", buf);

		sleep(1);
	}

	close(fd);
	return 0;
}

