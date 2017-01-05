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
map_t * mymap;
int print_result(char*, int);
void *reducer( void *ptr );
void *mapper( void *ptr);
int hash ( char* word, int r);
char* join3(char *s1, char *s2);

struct ma{
    int id;
    int r;
    int m;
    char** text;
} ;

int *finish;
                  
void word_count(int m, int r, char *address){
    FILE * fp= NULL;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    char* address1 = "/input";
    char* address2 = "/output";
    char **text = malloc(sizeof(char*) * m);
    
    int x = 0;
    for(x=0;x<m;x++){
        char *n = malloc (sizeof(char)*2);
        snprintf(n , 10, "%d", x);
        char *put = join3(address,address1);
        char *input = join3(put,n);
        /*creat the input file address*/
        fp =fopen(input,"r");
        if (fp == NULL)
            exit(EXIT_FAILURE);
        /*open the file*/
        char * result = "";
        while ((read = getline(&line, &len, fp)) != -1) {
            int length = strlen(line);
            line[length-1] = ' ';
            result = join3(result , line);
            /*join all the lines in one file together to one single line*/
        }
        text[x] = result;
        /*store the line in the corresponding line in the **text  */
        fclose(fp);
    }
    
    mymap = (map_t *)malloc(sizeof(map_t ) * r);
    int number=0;
    for(number =0; number <r;number ++)
        mymap[number] = hashmap_new();
    int q =0;    
    queue = (struct buffered_queue *)malloc(sizeof(struct buffered_queue) * r);
    for( q=0; q<r; q++){
       buffered_queue_init(&queue[q],20);
    }
    /*initialize the hashtable and the queue*/
    
    
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
   /*the information passed to the thread store in the struct*/
    
    
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
    /*creat and run the thread */
    
    int t = 0;
    for(t=0;t<r;t++){
        char *z = malloc (sizeof(char)*2);
        snprintf(z , 10, "%d", t);
        char *put = join3(address,address2);
        /*creat the input text address*/
        char *input = join3(put,z);
        fp = fopen (input, "w+");
        /*open the file*/
        int j =0;
        hashmap_map * m;
        m = (hashmap_map *) mymap[t];
        for(j = 0; j< m->table_size; j++){
            if(m->data[j].in_use != 0) {
                char *key = m->data[j].key;
                int data = m->data[j].data;
                fprintf(fp, "%s %d\n", key,data);
                /* copy one hashtable to on corresponding output file*/
            }
        }
        fclose(fp);
    }
    
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
    /*each mapper deals one line in **text, that is one file */
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
	error = hashmap_get(mymap[p.id], token, &count);
	if(error==MAP_OK)
		hashmap_put(mymap[p.id], token, count+1);
	else
		hashmap_put(mymap[p.id], token, 1);
    }
    return NULL;
    /*each reducer puts one queue to one hashtable*/
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

char* join3(char *s1, char *s2)  {
    char *result = malloc(strlen(s1)+strlen(s2)+1);
    if (result == NULL) exit (1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
    /*join two string to one string*/
}


