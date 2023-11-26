#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>  // Include this for memset

#define DEVICE_FILE	"/dev/mydevice"

int main()
{
	int fd;
	int retval;
	char buffer[10];

	printf("Opening File:%s\n", DEVICE_FILE);
	fd = open(DEVICE_FILE, O_RDWR);

	if (fd < 0) {
		perror("Open Failed");
		exit(1);
	}

	getchar();

	retval = write(fd, "hello", 5);
	printf("Write retval:%d\n", retval);
	getchar();

	retval = read(fd, buffer, 10);
	printf("Read retval:%d\n", retval);
	getchar();

    printf("Locking File, added by Eric\n");
    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK; // Write lock for exclusive access
    lock.l_whence = SEEK_SET; // Lock the whole file
    lock.l_start = 0;
    lock.l_len = 0;
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("fcntl");
        close(fd);
        exit(EXIT_FAILURE);
    }
	getchar();

    printf("Release the lock, added by Eric\n");
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
	getchar();
	
	printf("Closing File\n");
	close(fd);
	getchar();

	return 0;
}
