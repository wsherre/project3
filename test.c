#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(int argc, char** argv){

    int* ptr[10000];

    for(int i = 0; i < 10000; ++i){
        ptr[i] = malloc(i);
        fprintf(stdout, "Mallocing %p %d\n", ptr[i], i);
    }
    for(int i = 1; i < 10000; ++i){
        free(ptr[i]);
        fprintf(stdout, "Freeing %p %d\n", ptr[i], i);
    }
    free(ptr[0]);
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