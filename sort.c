#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

int array[10]={8,6,3,8,9,2,10,4,1,2};

void main_thread()
{
	int i,j,t;
	for (i=0;i<5;i++)
		for (j=i;j<5;j++)
			if (array[i]>array[j])
			{
				t=array[i];
				array[i]=array[j];
				array[j]=t;
			}
}

void *son_thread(void *argv)
{
	int i,j,t;
	for (i=5;i<10;i++)
		for (j=i;j<10;j++)
			if (array[i]>array[j])
			{
				t=array[i];
				array[i]=array[j];
				array[j]=t;
			}
}

void forge()
{
	int tmp[10];
	int i=0,j=5,t=0,k;
	while(i<5&&j<10)
		if (array[i]<array[j])
			tmp[t++]=array[i++];
		else
			tmp[t++]=array[j++];
	if (i==5)
		for (k=j;k<10;k++)
			tmp[t++]=array[k];
	if (j==10)
		for (k=i;k<5;k++)
			tmp[t++]=array[k];
	for (i=0;i<10;i++)
		array[i]=tmp[i];
}

int main()
{
	int i;
	pthread_t tid;
	printf("原始序列:");
	for (i=0;i<=9;i++)
		printf("%d ",array[i]);
	printf("\n");
	pthread_create(&tid,NULL,son_thread,NULL);
	main_thread();
	pthread_join(tid,NULL);
	printf("多线程处理后的序列:");
	for (i=0;i<=9;i++)
		printf("%d ",array[i]);
	printf("\n");
	forge();
	printf("排序序列:");
	for (i=0;i<=9;i++)
		printf("%d ",array[i]);
	printf("\n");
	return 0;
}