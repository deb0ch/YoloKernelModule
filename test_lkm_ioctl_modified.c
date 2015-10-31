
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
	char	buf[256];
	int	yolo_fd;
	int	count;

	if ((yolo_fd = open("/dev/yolo", O_RDWR)) == -1)
	{
		perror("open error");
		return -1;
	}
	printf("nmodified = %d\n", ioctl(yolo_fd, 0));
	if (close(yolo_fd) == -1)
	{
		perror("close error");
		return -1;
	}
}
