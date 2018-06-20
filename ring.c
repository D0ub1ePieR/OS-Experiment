#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

#define num 20
#define line 25

int res=0;
pthread_t tid[num];

void *pt(void *arg)
{
	int *i;
	i=(int *)arg;
	if (res>line)
		exit(0);
	if ((*i)==num-1)
	{
		printf("thread-%d	res=%d\n",*i,res);
		res++;
		*i=0;
		pthread_create(&tid[0],NULL,pt,i);
	}
	else
	{
		printf("thread-%d	res=%d\n",*i,res);
		res++;
		*i=(*i)+1;
		pthread_create(&tid[(*i)+1],NULL,pt,i);
	}
}

int main()
{
	int i=0;
	
	printf("线程数:%d	统计个数:%d\n",num,line);
	
	pthread_create(&tid[0],NULL,pt,&i);
	
	while(1);
	//pthread_join(tid[num-1],NULL);
	
	//printf("tid=%d	res=%d\n",getpid(),res);
}