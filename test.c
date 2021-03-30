#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char** argv){

    int* ptr[1000000];
    int k = 50000;


    for(int i = 1; i < k; ++i){
        ptr[i] = malloc(8);
        *ptr[i] = i;
        fprintf(stdout, "Mallocing %p %d %d\n", ptr[i], *ptr[i], i);
    }
    for(int i = 1; i < k; ++i){
        int num = rand() % 4000;
        ptr[i] = realloc(ptr[i], i);
        *ptr[i] = i;
        fprintf(stdout, "Reallocing %p %d %d\n", ptr[i], i, num);
    }
    for(int i = 1; i < k; ++i){
        free(ptr[i]);
        fprintf(stdout, "Freeing %p %d\n", ptr[i], i);
    }

    
    return 0;
}