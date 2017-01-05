#ifndef _BUFFERED_QUEUE_H
#define _BUFFERED_QUEUE_H
#include <pthread.h>

/* do not use any global variables in this file */

struct buffered_queue{
    int front;
    int rear;
    int* a;
    int size;
};

void buffered_queue_init(struct buffered_queue *queue, int size);

void buffered_queue_push(struct buffered_queue *queue, void *item);

void* buffered_queue_pop(struct buffered_queue *queue);

void buffered_queue_destroy(struct buffered_queue *queue);

#endif


