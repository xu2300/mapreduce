#include <stdio.h>
#include <stdlib.h>
#include <word_count.h>

int main(int argc, char **args) {
    if(argc<=3){
	printf("Usage: ./main [m] [r] [m strings]\n");
	return -1;
    }
    int m = atoi(args[1]);
    int r = atoi(args[2]);
    if(argc-3!=m){
	printf("Error: m=%d, but %d strings\n", m, argc-3);
	return -1;
    }
    char **text = malloc(sizeof(char*) * m);
     int i =0;
    for( i=0; i<m; i++)
	text[i] = args[i+3];
    printf("m=%d, r=%d\n", m, r);
 int j =0  ;
  for(j=0; j<m; j++)
	printf("%s\n", text[j]);
    word_count(m, r, text); 
    free(text);
    return 0;
}

