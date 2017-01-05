#include "buffered_queue.h"
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
struct buffered_queue *queue;
pthread_mutex_t mutex;
sem_t full, empty;
void *generate_message(void *ptr);
void *print_message( void *ptr );
int main(int argc, char *argv[]) {
    int st = 10000;
    int producerthreads = 0;
    int consumerthreads =0; 
    int BUFFER_SIZE = 0;
    printf("BUFFER_SIZE:");
    scanf("%d", &BUFFER_SIZE);
    printf("producerthreads:");
    scanf("%d", &producerthreads);
    printf("consumerthreads:");
    scanf("%d", &consumerthreads);
    sem_init(&empty, 0,BUFFER_SIZE);
    sem_init(&full, 0, 0);
   pthread_mutex_init(&mutex,NULL);
    /* initialize the queue */
    queue = (struct buffered_queue *)malloc(sizeof(struct buffered_queue));
    buffered_queue_init(queue,BUFFER_SIZE);
    pthread_t prod[producerthreads];
    pthread_t cons[consumerthreads];
    int a = 0;
    int b =0;
    int c=0;
    int d=0;
    int e=0;
    int f =0;
    for( a=0; a<producerthreads; a++){
        pthread_create(&prod[a], NULL, generate_message, NULL);
    }
    for( b=0; b<consumerthreads; b++){
        pthread_create(&cons[b], NULL, print_message, NULL);
    }
    for( c=0; c<producerthreads; c++){
        pthread_join(prod[c], NULL);
    }
    for( d=0; d<consumerthreads; d++){
        pthread_join(cons[d], NULL);
    }
       sleep(st);
    // Kills threads and ends.
    for (e = 0; e< producerthreads; ++e){
        pthread_cancel(prod[e]);
    }
    for (f = 0; f <consumerthreads; ++f){
        pthread_cancel(cons[f]);
    }
    free(queue);
    return 0;
}
/* this is the function executed by the producer thread.
 It should generate a number of messages and push them into the queue */
void *generate_message(void *ptr){
    int item;
    do {
        sleep(1);
        item = rand() % 10;
        sem_wait(&empty);
	pthread_mutex_lock(&mutex);        
	buffered_queue_push(queue, & item);
	int w = 0;	
	printf("queue: ");
	for(w=0; w<(queue->size);w++)
		printf("%d ", queue->a[w]);
	printf("\n");
	pthread_mutex_unlock(&mutex); 
        sem_post(&full);
	
            }while(true);
}
/* this is the function executed by the consumer thread.
 It should pop messages from the queue and print them */
void *print_message( void *ptr ){
    int *item;
    do {
        sleep(1);
        sem_wait(&full);
	pthread_mutex_lock(&mutex); 
        item = buffered_queue_pop(queue);
	int w = 0;	
	printf("queue: ");
	for(w=0; w<(queue->size);w++)
		printf("%d ", queue->a[w]);
	printf("\n");
	pthread_mutex_unlock(&mutex);    
        sem_post(&empty);
    }while(true);
}
