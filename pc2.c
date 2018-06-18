#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<ctype.h>

#define size 4
#define count 8

char buff[3][size];
int in[3]={0,0,0},out[3]={0,0,0};

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

typedef struct
{
	int value;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
}sema_t;

sema_t mutex_buff1,mutex_buff2;
sema_t wait_empty_buff1,wait_empty_buff2;
sema_t wait_full_buff1,wait_full_buff2;

void sema_init(sema_t *sema, int value)
{
	sema->value=value;
	pthread_mutex_init(&sema->mutex,NULL);
	pthread_cond_init(&sema->cond,NULL);
}

void sema_wait(sema_t *sema)
{
	pthread_mutex_lock(&sema->mutex);
	sema->value--;
	while(sema->value<0)
		pthread_cond_wait(&sema->cond,&sema->mutex);
	pthread_mutex_unlock(&sema->mutex);
}

void sema_signal(sema_t *sema)
{
	pthread_mutex_lock(&sema->mutex);
	sema->value++;
	pthread_cond_signal(&sema->cond);
	pthread_mutex_unlock(&sema->mutex);
}

//消费者
void *consume(void *arg)
{
	int i;
	char item;
	
	for (i=0;i<count;i++)
	{
		sema_wait(&wait_full_buff2);
		sema_wait(&mutex_buff2);
		item=out_set(2);
		printf("		consume item: %c\n",item);
		sema_signal(&mutex_buff2);
		sema_signal(&wait_empty_buff2);
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
		sema_wait(&wait_full_buff1);
		sema_wait(&mutex_buff1);
		item=out_set(1);
		sema_signal(&mutex_buff1);
		sema_signal(&wait_empty_buff1);
		
		sema_wait(&wait_empty_buff2);
		sema_wait(&mutex_buff2);
		in_set(2,toupper(item));
		printf("	calculate item: %c\n",toupper(item));
		sema_signal(&mutex_buff2);
		sema_signal(&wait_full_buff2);
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
		sema_wait(&wait_empty_buff1);
		sema_wait(&mutex_buff1);
		item='a'+i;
		in_set(1,item);
		printf("produce item: %c\n",item);
		sema_signal(&mutex_buff1);
		sema_signal(&wait_full_buff1);
	}
	return NULL;
}

int main()
{
	pthread_t tid1,tid2;
	sema_init(&mutex_buff1,1);
	sema_init(&mutex_buff2,1);
	sema_init(&wait_empty_buff1,3);
	sema_init(&wait_empty_buff2,3);
	sema_init(&wait_full_buff1,0);
	sema_init(&wait_full_buff2,0);
	
	pthread_create(&tid2,NULL,calculate,NULL);
	pthread_create(&tid1,NULL,consume,NULL);
	produce(NULL);
	pthread_join(tid2,NULL);
	pthread_join(tid1,NULL);
	return 0;
}