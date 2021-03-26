#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(int argc, char** argv){

    int* ptr[100000];
    

    /*for(int i = 4; i < 1000; i++){
        ptr[i] = malloc(i);
        *ptr[i] = i;
        fprintf(stdout, "%p\t%d\n", ptr[i], *ptr[i]);
    }
    for(int i = 4; i < 1000; i++){
        ptr[i] = realloc(ptr[i], i + 1);
        fprintf(stdout, "%p\t%d\n", ptr[i], *ptr[i]);
    }
    for(int i = 4; i < 1000; i++){
        free(ptr[i]);
    }

    for( int i = 0; i < 8; ++i){
        ptr[i] = calloc(1, 527);
        *ptr[i] = i * 1000;
        ptr[i] = realloc(ptr[i], 528);
        fprintf(stdout, "%p\t%d\n", ptr[i], *ptr[i]);
    }
    for( int i = 0; i < 8; ++i){
        free(ptr[i]);
    }
    for( int i = 4; i < 10000; ++i){
        ptr[i] = malloc(i);
        *ptr[i] = i;
        fprintf(stdout, "%p\t%d\n", ptr[i], *ptr[i]);
    }
    for( int i = 0; i < 10000; ++i){
        free(ptr[i]);
    }*/

    ptr[0] = malloc(1220);
    *ptr[0] = 1220;
    ptr[1] = malloc(3000);
    *ptr[1] = 3000;
    ptr[2] = malloc(8000);
    *ptr[2] = 8000;
    ptr[3] = malloc(12000);
    *ptr[3] = 12000;
    fprintf(stdout, "%p\t%d\n", ptr[0], *ptr[0]);
    fprintf(stdout, "%p\t%d\n", ptr[1], *ptr[1]);
    fprintf(stdout, "%p\t%d\n", ptr[2], *ptr[2]);
    fprintf(stdout, "%p\t%d\n", ptr[3], *ptr[3]);


    free(ptr[1]);
    free(ptr[0]);
    free(ptr[3]);
    free(ptr[2]);
    

    
    return 0;
}