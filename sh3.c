#include<stdio.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<stdlib.h>

#define buff 1024

int flag[buff];

void getcommand(int *argc, char *argv[buff])
{
	char tmp;
	int cur = 0, text_flag = 0, space_flag = 0;
	*argc = 0;
    while (1)
	{
		scanf("%c", &tmp);
		//换行命令结束
		if (tmp == '\n')
		{
			if (cur != 0)
			{
				argv[*argc][cur] = '\0';
				(*argc)++;
			}
			break;
		}
		//当前字符不为空格
		if (tmp != ' ')
		{
			if (cur == 0)
			{
				argv[*argc] = (char *)malloc(sizeof(char)*buff);
				memset(argv[*argc], 0, buff);
			}
			text_flag = 1;
			space_flag = 0;
			argv[*argc][cur++] = tmp;
		}
		else
			//读取到空格
			if (!space_flag && text_flag)
			{
				space_flag = 1;
				argv[*argc][cur] = '\0';
				cur = 0;
				(*argc)++;
			}
	}
}

void run_pipe(int start, int end, int fcnt, char *argv[])
{
	pid_t pid;
	int i,err,fd[2];
	char *tmp[buff],t;
	if (end != start+1)
	{
        pipe(fd);    
		pid=fork();
		if (pid==0)
		{
			if (end-1 !=fcnt-1)
			{
				dup2(fd[1],1);
				close(fd[1]);
				close(fd[0]);
			}
			run_pipe(start,end-1,fcnt,argv);
		}
		else
			waitpid(pid,NULL,0);
	}
	for (i=flag[end-1]+1;i<=flag[end]-1;i++)
		tmp[i-flag[end-1]-1]=argv[i];
    if (end!=start+1)
	{
		dup2(fd[0],0);
		close(fd[0]);
		close(fd[1]);
	}
	err=execvp(tmp[0],tmp);
	if (err<0)
		perror("execvp");
	if (end!=fcnt-1)
		close(1);
}

/*int exec(int argc,char *argv[])
{
	int i,j,fcnt=0,err;
	flag[fcnt++]=-1;
	for (i=0;i<argc;i++)
		if (strcmp(argv[i],"|")==0)
			flag[fcnt++]=i;
	flag[fcnt++]=argc;
	for (i=1;i<fcnt;i++)
		if (flag[i]==flag[i-1]+1)
			return -1;
	pid_t pid;
	pid=fork();
	if (pid==0)
    {
		run_pipe(0,fcnt-1,fcnt,argv);
    }
	else
	{
		wait(NULL);
		printf("command done\n");
	}
	return 0;
}*/

void mysys(int argc, char *argv[])
{
	pid_t pid;
	char tmp[buff],t;
	int err,fd[2],file,fflag=0;
	if (argc > 3 && strcmp(argv[argc-2],">")==0)
    {
		fflag=1;
		strcpy(tmp,argv[argc-1]);
	}
	
	int i,j,fcnt=0;
	flag[fcnt++]=-1;
	for (i=0;i<argc;i++)
		if (strcmp(argv[i],"|")==0)
			flag[fcnt++]=i;
	flag[fcnt++]=argc;
	for (i=1;i<fcnt;i++)
		if (flag[i]==flag[i-1]+1)
			return;
	
	pid=fork();
	if (pid==0)
	{
		if (fflag)
		{
			pid_t ppid;
			pipe(fd);
			ppid=fork();
			if (ppid==0)
			{
				dup2(fd[1],1);
				close(fd[0]);
				close(fd[1]);
				argv[argc-2]=NULL;
				argv[argc-1]=NULL;
				run_pipe(0,fcnt-1,fcnt,argv);
				//err=exec(argc, argv);
				//if (err<0)
				//	perror("execvp");
			}
			else
			{
				wait(NULL);
				dup2(fd[0],0);
				close(fd[0]);
				close(fd[1]);
				file=open(tmp,O_RDWR|O_CREAT,0666);
				if (file<0)
				{
					printf("cannot open file!\n");
					exit(0);
				}
				while(1)
				{
					if (!read(0,&t,sizeof(char)))
						break;
					write(file,&t,sizeof(char));
				}
			}
            return;
		}
		else
		{
			run_pipe(0,fcnt-1,fcnt,argv);
			//err=exec(argc, argv);
			//if (err<0)
			//	perror("execvp");
		}
		return;
	}
	else
	{
		wait(NULL);
	}	
	return;	
}

int main()
{
	int i, argc;
	char pwd[buff], *argv[buff];
	FILE *shell;
	while (1)
	{
        argc=0;
		//输出每行前的表示符
		shell=popen("/bin/pwd","r");
		fscanf(shell,"%s",pwd);
		pclose(shell);
		printf("$%s>",pwd);		
		//用户输入命令
		getcommand(&argc, argv);
        //用户未输入
		if (argc==0)
			continue;		
		//输入exit指令 退出当前程序
		if (strcmp(argv[0],"exit")==0)
			return 0;
		if (strcmp(argv[0],"cd")==0)
		{	
			if (chdir(argv[1]))
				printf("no such direction : %s\n",argv[1]);
		}	
		else
			if (strcmp(argv[0],"pwd")==0)
			{
				char ppwd[buff];
        		printf("%s\n",getcwd(ppwd,sizeof(ppwd)));
			}
			else
			{
				mysys(argc,argv);
			}
		memset(argv,0,sizeof(argv));
  	}
	return 0;
}
