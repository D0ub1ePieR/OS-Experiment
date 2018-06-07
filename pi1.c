#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<math.h>

#define buff 50000

float res=0,res_fst=0,res_sec=0;
int array[buff];

void init_array(int precision)
{
	int i;
	for (i=1;i<=precision;i++)
		if (i%2==1)
			array[i]=2*(i-1)+1;
		else
			array[i]=-2*(i-1)-1;
}

void main_thread(int precision)
{
	int i;
	for (i=1;i<=(precision+1)/2;i++)
		res_fst+=1.0/array[i];
}

void *son_thread(void *argv)
{
	int *num,i;
	num=argv;
	for (i=(*num+1)/2+1;i<=*num;i++)
		res_sec+=1.0/array[i];
}

int main()
{
	int precision=40000;
	pthread_t tid;
	
	init_array(precision);
	pthread_create(&tid,NULL,son_thread,&precision);
	main_thread(precision);
	pthread_join(tid,NULL);
	res=res_fst+res_sec;
	printf("res_fst = %f, res_sec = %f, res = %f, PI = %f (precision:%d)\n",res_fst,res_sec,res,res*4,precision);
	return 0;
}