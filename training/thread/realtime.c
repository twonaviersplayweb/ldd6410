#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <sys/types.h>

struct thread_param
{
	char info;
	int num;
};

void* thread_fun(void* param)
{
	pthread_attr_t   attr; 
	struct  sched_param  s_param; 
	int newprio=20;

	pthread_attr_init(&attr);
	pthread_attr_setschedpolicy(&attr, SCHED_FIFO);  
	pthread_attr_getschedparam(&attr, &s_param);
	s_param.sched_priority=newprio;
	pthread_attr_setschedparam(&attr, &s_param);

	printf("thread pid:%d, tid:%lu\n",getpid(), pthread_self());

	return NULL;
}

int main(void)
{
	pthread_t tid1,tid2;
	struct thread_param info1;
	struct thread_param info2;
	int ret;

	info1.info='T';
	info1.num=2000000;

	printf("main pid:%d, tid:%lu\n",getpid(), pthread_self());

	ret=pthread_create(&tid1,NULL,thread_fun,&info1);
	if(ret==-1){
		perror("cannot create new thread");
		return 1;
	}

	info2.info='S';
	info2.num=300000;

	ret=pthread_create(&tid2,NULL,thread_fun,&info2);
	if(ret==-1){
		perror("cannot create new thread");
		return 1;
	}

	if(pthread_join(tid1,NULL)!=0){
		perror("call pthread_join function fail");
		return 1;
	}

	if(pthread_join(tid2,NULL)!=0){
		perror("call pthread_join function fail");
		return 1;
	}
	return 0;
}	
