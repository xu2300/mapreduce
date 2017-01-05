#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "word_count.h"
#include "hashmap.h"
#include "buffered_queue.h"
#include <unistd.h>
#include <stdbool.h>


int main(int argc, char **args) {

    if(argc<=3){
        printf("Usage: ./main [m] [r] [m strings]\n");
        return -1;
    }
    int m = atoi(args[1]);
    int r = atoi(args[2]);
    char *address= args[3];
    
    word_count(m, r, address);
    return 0;
}




