#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>

void mysys(int argc, char *argv[])
{
	pid_t pid;
	pid=fork();
	if (pid==0)
	{
		execvp(argv[1], argv+1);
	}
	else
	{
		wait(NULL);
		printf("--------------------------------------------------\n");
	}		
}

int main(int argc, char *argv[])
{
	printf("--------------------------------------------------\n");
	mysys(argc,argv);
	return 0;
}
