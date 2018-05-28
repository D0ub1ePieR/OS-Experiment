#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<stdlib.h>

#define buff 1024

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

void mysys(int argc, char *argv[])
{
	pid_t pid;
	int err;
	pid=fork();
	if (pid==0)
	{
		err=execvp(argv[0], argv);
		if (err<0)
			perror("execvp");
	}
	else
	{
		wait(NULL);
	}		
}

int main()
{
	int argc, i;
	char *argv[buff],pwd[buff];
	FILE *shell;
	while (1)
	{
		argc = 0;
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
			exit(0);
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
				//if (strcmp(argv[0],"ls")==0 && (argc==1 || (argc!=1 && argv[1][0]=='-')))
					//argv[argc++]=pwd;
				//printf("qqq~!%d",argc);
				//for (i=0;i<argc;i++)
				//	printf("!%s ",argv[i]);
				//printf("\n");
				mysys(argc,argv);
			}
		for (i=0;i<argc;i++)
            argv[i]=NULL;
			//memset(argv[i],0,buff);
	}
	return 0;
}
