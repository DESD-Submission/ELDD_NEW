#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int main(void)
{
	int ret;
	char data1[32];
	char data2[32];
	char data3[32];

	int fd = open("/dev/pchar0", O_RDONLY);

	ret = read(fd, data1, 26);
	printf("bytes read = %d, data = %s\n", ret, data1);

	ret = read(fd, data2, 10);
	printf("bytes read = %d, data = %s\n", ret, data2);
	
	ret = read(fd, data3, 5);
	printf("bytes read = %d, data = %s\n", ret, data3);
	
	close(fd);

	return 0;
}
