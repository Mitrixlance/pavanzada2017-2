#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include "hellow.h"

#define NUM_THREADS 10

unsigned int total;
sem_t forks[NUM_THREADS];

void thinking(int n){
	sleep(rand()%n);
}

void eating(int n){
	sleep(rand()&n);
}

void *lifeStyle(void *threadid) {
    long tid;
    tid = (long)threadid;

    while(1){
    	thinking(3);
    	printf("I am phil %lu, I am hungry \n", tid);
    	if(tid % 2 == 0){
    		sem_wait(&forks [tid]);
    		sem_wait(&forks [(tid + 1) %NUM_THREADS]);
        } else {
        	sem_wait(&forks [(tid + 1) %NUM_THREADS]);
        	sem_wait(&forks [tid]);
        }


    	eating(4);
    	sem_post(&forks [tid]);
    	sem_post(&forks [(tid + 1) %NUM_THREADS]);
    	printf("I am phil %lu, I am thinking \n", tid);
    }
    
 }

 int main (int argc, char *argv[]) {
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;

    for(t=0; t<NUM_THREADS; t++){
    	sem_init(&forks[t],0,1);	
    }

    for(t=0; t<NUM_THREADS; t++){	
       printf("In main: creating thread %ld\n", t);
       rc = pthread_create(&threads[t], NULL, lifeStyle, (void *)t);
       if (rc){
          printf("ERROR; return code from pthread_create() is %d\n", rc);
          exit(-1);
       }   
    }
    for(t=0; t<NUM_THREADS; t++){

    	pthread_join(threads[t],NULL);
	}

    /* Last thing that main() should do */
    pthread_exit(NULL);
 }
