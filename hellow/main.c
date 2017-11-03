#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include "hellow.h"

#define NUM_THREADS 1000

unsigned int total;
sem_t sem1;

void *add100(void *threadid) {
    long tid;
    tid = (long)threadid;
    unsigned int localTotal;
    printf("Adding  100 #%ld!\n", tid);
    sem_wait(&sem1);
    localTotal = total;
    localTotal += 100;
    //sleep(tid%3);
    total = localTotal;
    sem_post(&sem1);
    pthread_exit(NULL);
 }

 int main (int argc, char *argv[]) {
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;
    total = 0;
    sem_init(&sem1, 0, 1);
    printf("Initial total = %d \n", total);
    for(t=0; t<NUM_THREADS; t++){	
       printf("In main: creating thread %ld\n", t);
       rc = pthread_create(&threads[t], NULL, add100, (void *)t);
       if (rc){
          printf("ERROR; return code from pthread_create() is %d\n", rc);
          exit(-1);
       }   
    }
    for(t=0; t<NUM_THREADS; t++){

    	pthread_join(threads[t],NULL);
	}
    printf("Final total = %d \n", total);


    /* Last thing that main() should do */
    pthread_exit(NULL);
 }
