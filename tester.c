#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

int main()
{
	pid_t pid;
	if((pid = fork()) == 0)
	{
		printf("[1] %d\n", getpid());
		char* args[2];
		args[0] = "./a.out";
		args[1] = 0;
		execv(args[0], args);
	}
	else
	{
		int status;
		waitpid(pid, &status, 0);
		printf("end\n");
	}
}
