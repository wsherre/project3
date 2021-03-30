#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>

int main(int argc, char** argv){

    int* ptr[1000000];
    time_t t;
    srand((unsigned) time(&t));


    /*for(int i = 1; i < k; ++i){
        ptr[i] = malloc(k);
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
        c
    }*/
    for(int i = 0; i < 10000; ++i){
        ptr[i] = malloc(8);
        memset(ptr[i], i, 8);
        fprintf(stdout, "Mallocing %p %d %d\n", ptr[i], *ptr[i], i);
    }

    /*for(int i = 0; i < 100; ++i){
        ptr[i] = NULL;
    }
    for( int i = 0; i < 100000; ++i){
        int index = rand() % 100;
        int num = rand() % 5000 + 4;
        if(ptr[index] == NULL){
            ptr[index] = malloc(num);
            memset(ptr[index], 1, num);
            fprintf(stdout, "Mallocing %p %d %d\n", ptr[index], *ptr[index], index);
        }else{
            if(num < 2500){
                ptr[index] = realloc(ptr[index], rand() % 6000 + 4);
                memset(ptr[index], 1, num);
                fprintf(stdout, "Reallocing %p %d %d\n", ptr[index], *ptr[index], index);
            }else{
                free(ptr[index]);
                fprintf(stdout, "Freeing %p %d\n", ptr[index], index);
                ptr[index] = NULL;
            }
        }
    }*/

    
    return 0;
}