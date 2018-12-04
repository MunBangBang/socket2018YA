#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void *func_pthread(void *);

int main(){
    int status;
    pthread_t thread_id;
    
    status = pthread_create(&thread_id, NULL, func_pthread, NULL);
    if(status != 0){
        printf("[ERROR] pthread_create Failed\n");
        exit(1);
    }
    
    printf("thread %x is created\n", thread_id);
    pthread_join(thread_id, (void **)&status);
   // sleep(5);
    printf("main function exit\n");
	return 0;
}

void *func_pthread(void *arg){
    int i=0;
    
    while(i<10){
        sleep(1);
        i++;
        printf("thread %dth executing...\n", i);
	sleep(1);
    }
}
