#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int main(void)
{
	int ret;
	char data1[32] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char data2[32] = "0123456789";
	char data3[32] = "12345";

	int fd = open("/dev/pchar0", O_WRONLY);

	ret = write(fd, data1, 26);
	printf("bytes written = %d\n", ret);

	ret = write(fd, data2, 10);
	printf("bytes written = %d\n", ret);

	ret = write(fd, data3, 5);
	printf("bytes written = %d\n", ret);
	
	close(fd);

	return 0;
}
