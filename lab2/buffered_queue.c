#include "buffered_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
/*Do not use any global variables for implementation*/
/* initiliaze all necessary variables for the queue */
void buffered_queue_init(struct buffered_queue *queue, int size){
    queue->a = (int *)malloc((size+1) *sizeof(int));
    queue->size = size;
    queue->front = 0;
    queue->rear = 0;
}
/* push an item into the tail of the queue. If the queue is full, wait until someone pops an item */
void buffered_queue_push(struct buffered_queue *queue, void *item){
    printf("producer produced %d\n", *(int*)item);
    queue->a[queue->rear] = *(int*)item;
    queue->rear = (queue->rear + 1) % (queue->size);
    return ;
}
/* pop an item from the top of the queue. If the queue is empty, wait until someone pushes an item */
void* buffered_queue_pop(struct buffered_queue *queue){
    int *p;
    p = (int *)malloc(sizeof(int));
   *p = queue->a[queue->front];
    queue->a[queue->front] = 0;
    queue->front = (queue->front + 1) % (queue->size);
    printf("consumer consumed %d\n",*p);
    return p;
}
/* release all resources */
void buffered_queue_destroy(struct buffered_queue *queue){
 queue->front = queue->rear = 0;
}
