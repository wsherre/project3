#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <assert.h>

int main(int argc, char** argv){

    char* ptr[1000000];


    //if you wwant true randomness uncomment these two lines, but it'll make it harder to debug
    //time_t t;
    //srand((unsigned) time(&t));


    
    char n[400];
    //assert that memcpy works
    for(int i = 0; i < 400; ++i){
        ptr[i] = malloc(16);
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
    //code will seem slow but that's only because it takes printf forever to print out
    for( int i = 0; i < 150000; ++i){
        //randomly pick a pointer
        int index = rand() % 100;
        int num = rand() % 2000 + 1;
        //if its null randomly allocate a random amount of memory
        if(ptr[index] == NULL){
            ptr[index] = malloc(num);
            *ptr[index] = num % 128 + 32;
            fprintf(stdout, "Mallocing Pointer: %p  Value: %c Random num: %d Index: %d Operation num: %d\n", ptr[index], *ptr[index], num, index, i);
        }else{
            //else if the random is below 1000, randomly reallocate new memory
            if(num < 1000){
                int num = rand() % 2000 + 1;
                char test = *ptr[index];
                ptr[index] = realloc(ptr[index], num);
                *ptr[index] = num % 128 + 32;
                assert(*ptr[index] == test);
                fprintf(stdout, "Reallocing Pointer: %p  Value: %c Random num: %d Index: %d Operation num: %d\n", ptr[index], *ptr[index], num, index, i);
            }else{
                free(ptr[index]);
                fprintf(stdout, "Freeing Pointer: %p Index: %d\n", ptr[index], index);
                ptr[index] = NULL;
            }
        }
    }
    for(int i = 0; i < 100; ++i){
        if(ptr[i] != NULL){
            free(ptr[i]);
            fprintf(stdout, "Freeing %p %d\n", ptr[i], i);
        }
    }

    
    return 0;
}