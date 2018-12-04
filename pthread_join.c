#include <stdio.h>
#include <pthread.h>

void *do_sum(void *val); //1부터 val 까지의 합을 계산하는 함수
int main(){
	int status;
	pthread_t p_thread[3];
	int a=4;
	int b=5;
	int c=6;

	status = pthread_create(&p_thread[0],NULL,do_sum, (void *)&a);//a가 int값이므로 주소값으로 반환
	status = pthread_create(&p_thread[1],NULL,do_sum, (void *)&b);
	status = pthread_create(&p_thread[2],NULL,do_sum, (void *)&c);
	
	pthread_join(p_thread[0], (void **)&status);
	pthread_join(p_thread[1], (void **)&status);
	pthread_join(p_thread[2], (void **)&status);
	printf("main program end\n");
	return 0;
}

void *do_sum(void *val)
{
	int i;
	int sum=0;
	int max = *((int *)val);

	for(i=0;i<max;i++){
		sum=sum+i;
		printf("%d -Add %d\n", max,i);
		sleep(1);
	}
	printf("%d - sum(%d)\n",max,sum);
}