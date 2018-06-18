#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<ctype.h>

#define size 4
#define count 8

char buff[3][size];
int in[3]={0,0,0},out[3]={0,0,0};

//检查缓冲区是否为空
int check_empty(int num)
{
	return in[num]==out[num];
}

//检查缓冲区是否已满
int check_full(int num)
{
	return (in[num]+1)%size==out[num];
}

//入缓冲区
void in_set(int num,char item)
{
	buff[num][in[num]]=item;
	in[num]=(in[num]+1)%size;
}

//出缓冲区
char out_set(int num)
{
	char item;
	item=buff[num][out[num]];
	out[num]=(out[num]+1)%size;
	return item;
}

pthread_mutex_t mutex_buff1,mutex_buff2;
pthread_cond_t wait_empty_buff1,wait_empty_buff2;
pthread_cond_t wait_full_buff1,wait_full_buff2;

//消费者
void *consume(void *arg)
{
	int i;
	char item;
	for (i=0;i<count;i++)
	{
		pthread_mutex_lock(&mutex_buff2);
		while (check_empty(2))
			pthread_cond_wait(&wait_full_buff2,&mutex_buff2);
		item = out_set(2);
		printf("		consume item: %c\n",item);
		
		pthread_cond_signal(&wait_empty_buff2);
		pthread_mutex_unlock(&mutex_buff2);
	}
	return NULL;
}

//计算者
void *calculate(void *arg)
{
	int i;
	char item;
	for (i=0;i<count;i++)
	{
		pthread_mutex_lock(&mutex_buff1);
		while(check_empty(1))
			pthread_cond_wait(&wait_full_buff1,&mutex_buff1);
		item=out_set(1);
		pthread_cond_signal(&wait_empty_buff1);
		pthread_mutex_unlock(&mutex_buff1);
		
		pthread_mutex_lock(&mutex_buff2);
		while(check_full(2))
			pthread_cond_wait(&wait_empty_buff2,&mutex_buff2);
		in_set(2,toupper(item));
		printf("	calculate item: %c\n",toupper(item));
		pthread_cond_signal(&wait_full_buff2);
		pthread_mutex_unlock(&mutex_buff2);
	}
	return NULL;
}

//生产者
void *produce(void *arg)
{
	int i;
	char item;
	for (i=0;i<count;i++)
	{
		pthread_mutex_lock(&mutex_buff1);
		while (check_full(1))
			pthread_cond_wait(&wait_empty_buff1,&mutex_buff1);
		item='a'+i;
		in_set(1,item);
		printf("produce item: %c\n",item);
		pthread_cond_signal(&wait_full_buff1);
		pthread_mutex_unlock(&mutex_buff1);
	}
	return NULL;
}

int main()
{
	pthread_t tid1,tid2;
	pthread_mutex_init(&mutex_buff1,NULL);
	pthread_mutex_init(&mutex_buff2,NULL);
	pthread_cond_init(&wait_empty_buff1,NULL);
	pthread_cond_init(&wait_empty_buff2,NULL);
	pthread_cond_init(&wait_full_buff1,NULL);
	pthread_cond_init(&wait_full_buff2,NULL);
	
	pthread_create(&tid1,NULL,consume,NULL);
	pthread_create(&tid2,NULL,produce,NULL);
	calculate(NULL);
	pthread_join(tid2,NULL);
	pthread_join(tid1,NULL);
	return 0;
}