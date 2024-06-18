#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int main(void)
{
	int ret;
	char data[16] = "Hello DESD";
	char result[16];

	int fd = open("/dev/pchar0", O_RDWR);

	ret = write(fd, data, sizeof(data));
	printf("bytes written = %d\n", ret);

	ret = read(fd, result, sizeof(result));
	printf("bytes read = %d\n", ret);
	printf("result = %s\n", result);

	close(fd);

	return 0;
}
