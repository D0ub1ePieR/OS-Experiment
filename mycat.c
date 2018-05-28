#include<stdio.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{
    int fd;
    char buff;
    if (argc<2)
    {
        printf("please give a filename!\n");
	exit(0);
    }
    fd=open(argv[1],O_RDONLY);
    if (fd==-1)
    {
        printf("can't open the file!\n");
		exit(0);
    }
    while(1)
   {
		if (!read(fd,&buff,sizeof(char)))
			break;
		printf("%c",buff);	
	}
}
