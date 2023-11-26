#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#define DIR_TO_MAKE "lwl"

int main(void)
{
	getchar();
	if(mkdir(DIR_TO_MAKE, 0700) == -1)
	{
		printf("Failed to create %s\n", DIR_TO_MAKE);
		perror("mkdir");
		exit(EXIT_FAILURE);
	}
	else
		printf("Created directory: %s\n", DIR_TO_MAKE);

	return 0;
	
}
