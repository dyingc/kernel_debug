#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#define UTILITY_NAME "cp_mv_del"
#define BUF_SIZE    1024


static bool copy_file(char *src_path, char *dest_path)
{
    int src_fd;
    int dest_fd;
    char buf[BUF_SIZE];
    ssize_t num_bytes_read;
    struct stat st;
    bool retval = false;

    if (!src_path || !dest_path) {
        printf("%s: Invalid arguments passed\n", UTILITY_NAME);
        return false;
    }

	//open src file in read mode, dest file in create mode
    src_fd = open(src_path, O_RDONLY);
    if (src_fd < 0) {
        printf("%s: Failed to open %s for reading, errno:%d\n",
                UTILITY_NAME, src_path, errno);
        return false;
    }
    dest_fd = open(dest_path, O_CREAT | O_WRONLY | O_TRUNC);
    if (dest_fd < 0) {
        printf("%s: Failed to open %s for writing, errno:%d\n",
                UTILITY_NAME, dest_path, errno);
        goto exit;
    }
    while ((num_bytes_read = read(src_fd, buf, BUF_SIZE)) > 0) {
        if (write(dest_fd, buf, num_bytes_read) != num_bytes_read) {
            printf("%s: Failed to write the whole buffer\n", UTILITY_NAME);
            goto exit;
        }
    }
    //Copy permissions
    if (stat(src_path, &st) != 0) {
        printf("%s: Failed to get file:%s status\n", UTILITY_NAME, src_path);
        goto exit;
    }
    if (chmod(dest_path, st.st_mode) != 0) {
        printf("%s: Failed to change mode of file%s\n", UTILITY_NAME, dest_path);
        goto exit;
    }
	retval = true;
exit:
    if (src_fd >= 0)
        close(src_fd);
    if (dest_fd >= 0)
        close(dest_fd);
    return retval;
}

static bool mv(char *oldfile, char *newfile)
{

	if (!oldfile || !newfile) {
        printf("%s: Invalid arguments passed\n", UTILITY_NAME);
        return false;
	}
	//check whether old file exist or not
	if (access(oldfile, F_OK) == -1) {
		printf("%s: File doesn't exist\n", UTILITY_NAME);
		return false;
	}	
	if (rename(oldfile, newfile) == -1) {
		perror("Failed to rename");
		return false;
	}
	return true;
}

static bool del(char *path)
{
	if (!path) {
        printf("%s: Invalid arguments passed\n", UTILITY_NAME);
        return false;
	}
	// check whether the path exists
	if (access(path, F_OK) == -1) {
		printf("%s: File doesn't exist\n", UTILITY_NAME);
		return false;
	}
	if (unlink(path) == -1) {
		perror("Failed to delete");
		return false;
	}
	return true;
}	

int main(int argc, char *argv[])
{
	getchar();
	if (copy_file("hello.txt", "world.txt")) {
		printf("Copy Successful\n");
	} else {
		printf("Copy failed\n");
	}

	getchar();
	if (mv("hello.txt", "file.txt")) {
		printf("Move successful\n");
	} else {
		printf("Move Failed\n");
	}

	getchar();
	if (del("world.txt")) {
		printf("Delete Successful\n");
	} else {
		printf("Delete failed\n");
	}
	return 0;
}
