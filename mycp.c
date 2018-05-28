#include<stdio.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{
	int fd_old,fd_new;
	char buff;
	if (argc<3)
	{
		printf("please give the source filename and the target name!\n");
		exit(0);
	}
	fd_old=open(argv[1], O_RDONLY);
	fd_new=open(argv[2], O_CREAT|O_RDWR, 0777);
	if (fd_old==-1 || fd_new==-1)
	{
		printf("can't open file!\n");
		exit(0);
	}
	while(1)
	{
		if (!read(fd_old, &buff, sizeof(char)))
			break;
		write(fd_new, &buff, sizeof(char));
	}
}
