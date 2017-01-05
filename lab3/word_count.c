#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "word_count.h"
#include "hashmap.h"
#include "buffered_queue.h"
#include <unistd.h>
#include <stdbool.h>

struct buffered_queue *queue;
map_t mymap;
int print_result(char*, int);
void *reducer( void *ptr );
void *mapper( void *ptr);
int hash ( char* word, int r);

struct ma{
    int id;
    int r;
    int m;
    char** text;
} ;

int *finish;

void word_count(int m, int r, char **text){
    
    mymap = hashmap_new();
    int q =0;    
    queue = (struct buffered_queue *)malloc(sizeof(struct buffered_queue) * r);
    for( q=0; q<r; q++){
       buffered_queue_init(&queue[q],20);
    }

    finish=(int *)malloc(sizeof(int) * m);
    int ff=0;
    for (ff=0; ff<m; ff++) {
        finish[ff]=1;
    }
    
    int num =0;
    if (m>=r) 
        num =m;
    else
        num = r;

   struct ma arg[num];
    int z =0;  
    for( z=0; z<num; z++){
       arg[z].text = (char **)malloc(sizeof(char*) * m);
       arg[z].id = z;
       arg[z].r = r;
       arg[z].m = m;
       arg[z].text  = text;
    }  

    pthread_t map[m];
    pthread_t re[r];  
    int a = 0;
    int b =0;
    int c=0;
    int d=0;
 
    for( a=0; a<m; a++){
        pthread_create(&map[a], NULL, mapper, &arg[a]);
    }
    for( b=0; b<r; b++){
        pthread_create(&re[b], NULL, reducer, &arg[b]);
    }
    for( c=0; c<m; c++){
        pthread_join(map[c], NULL);
    }
    for( d=0; d<r; d++){
        pthread_join(re[d], NULL);
    }

    buffered_queue_destroy(queue);
    printf("Result:\n");
    hashmap_iterate(mymap, print_result);
    return ;
}


void *mapper(void *ptr){
    char *flag = "1";
   struct ma p = *(struct ma *)ptr;
    const char delimiter[2] = " ";
    char *token;
    char *saveptr;    
     token = (char*)malloc(sizeof(char) *10);
    token = strtok_r(p.text[p.id], delimiter, &saveptr);
	while( token != NULL ) {
	    int value = hash(token, p.r);
	    buffered_queue_push(&queue[value], token);
      	    token = strtok_r(NULL, delimiter, &saveptr);
   	}
     int j =0;
     for(j=0;j<p.r;j++){
        buffered_queue_push(&queue[j], flag);
     }
     return NULL;
}

void *reducer( void *ptr ){
struct ma p = *(struct ma *)ptr;
int num = 0;
char *flag = "1";
int error;
   while(1){
        char * token = (char *)buffered_queue_pop(&queue[p.id]);
	if(!strcmp(token,flag)){
		num++;
		if(num == p.m)
			return NULL;
		continue;
        }
        int count;
	error = hashmap_get(mymap, token, &count);
	if(error==MAP_OK)
		hashmap_put(mymap, token, count+1);
	else
		hashmap_put(mymap, token, 1);
    }
    return NULL;
}

int hash ( char* word, int r)
{
    int hash = 0;
    int n;
    int i ;
    for (i = 0; word[i] != '\0'; i++)
    {
           n = word [i] - 'a' + 1;
           hash = ((hash << 3) + n) % r;
    }
    return hash;
}


int print_result(char *key, int value){
    printf("count of %s = %d\n", key, value);
    return 0;
}



