#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<math.h>

#define buff 50000

float res=0;
int array[buff];
struct param
{
	int start;
	int end;
};

void init_array(int precision)
{
	int i;
	for (i=1;i<=precision;i++)
		if (i%2==1)
			array[i]=2*(i-1)+1;
		else
			array[i]=-2*(i-1)-1;
}

void *son_thread(void *argv)
{
	int i;
	struct param *p;
	p=(struct param *)argv;
	for (i=p->start;i<=p->end;i++)
		res+=1.0/array[i];
}

int main()
{
	int precision=40000,n,i,step;
	pthread_t tid[20];
	
	init_array(precision);
	printf("计算进程数n=");
	scanf("%d",&n);
	step=precision/n;
	for (i=1;i<=n;i++)
	{
		struct param *p;
		p=(struct param *)malloc(sizeof(struct param));
		if (i!=n)
		{
			p->start=1+(i-1)*step;
			p->end=i*step;
		}
		else
		{
			p->start=1+(i-1)*step;
			p->end=precision;
		}
		pthread_create(&tid[i],NULL,son_thread,p);
	}
	for (i=1;i<=n;i++)
		pthread_join(tid[i],NULL);
	printf("res = %f, PI = %f (precision:%d)\n",res,res*4,precision);
	return 0;
}