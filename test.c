#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <assert.h>

int main(int argc, char** argv){

    char* ptr[1000000];
    time_t t;
    //srand((unsigned) time(&t));


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
    /*for(int i = 0; i < 10000; ++i){
        ptr[i] = malloc(8);
        memset(ptr[i], i, 8);`
        fprintf(stdout, "Mallocing %p %d %d\n", ptr[i], *ptr[i], i);
    }*/
    char n[400];
    for(int i = 0; i < 400; ++i){
        ptr[i] = malloc(8);
        *ptr[i] = i % 256;
        n[i] = i % 256;
        //fprintf(stdout, "Mallocing %p %d\n", ptr[i], *ptr[i]);
    }
    for(int i = 0; i < 400; ++i){
        ptr[i] = realloc(ptr[i], 8);
        assert(*ptr[i] == n[i]);
    }
    for(int i = 0; i < 400; ++i){
        free(ptr[i]);
    }
    for(int i = 0; i < 400; ++i){
        ptr[i] = NULL;
    }
    for( int i = 0; i < 5000000; ++i){
        int index = rand() % 100;
        int num = rand() % 5000 + 1;
        if(ptr[index] == NULL){
            ptr[index] = malloc(num);
            *ptr[index] = 0;
            //fprintf(stdout, "Mallocing %p %d %d %d %d\n", ptr[index], *ptr[index], num, index, i);
        }else{
            if(num < 512){
                int num = rand() % 5000 + 1;
                ptr[index] = realloc(ptr[index], num);
                *ptr[index] = 0;
                //fprintf(stdout, "Reallocing %p %d %d %d %d\n", ptr[index], *ptr[index], num, index, i);
            }else{
                free(ptr[index]);
                //fprintf(stdout, "Freeing %p %d\n", ptr[index], index);
                ptr[index] = NULL;
            }
        }
    }
    for(int i = 0; i < 100; ++i){
        if(ptr[i] != NULL){
            free(ptr[i]);
            //fprintf(stdout, "Freeing %p %d\n", ptr[i], i);
        }
    }
    
    return 0;
}