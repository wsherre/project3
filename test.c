#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(int argc, char** argv){

    int* ptr[679];
    /*for(int i = 0; i < 679; ++i){
        ptr[i] = malloc(8);
        *ptr[i] = i;
        fprintf(stdout, "%p\t%d\n", ptr[i], *ptr[i]);
    }
    for(int i = 678; i >= 0; --i){
        free(ptr[i]);
    }*/

    ptr[0] = malloc(1025);
    *ptr[0] = 500;
    fprintf(stdout, "%p\t%d\n", ptr[0], *ptr[0]);
    ptr[1] = malloc(4093);
    *ptr[1] = 1000;
    fprintf(stdout, "%p\t%d\n", ptr[1], *ptr[1]);
    ptr[2] = malloc(8000);
    *ptr[2] = 15000;
    fprintf(stdout, "%p\t%d\n", ptr[2], *ptr[2]);

    ptr[0] = realloc(ptr[0], 800);
    fprintf(stdout, "%p\t%d\n", ptr[0], *ptr[0]);
    ptr[1] = realloc(ptr[1], 5000);
    fprintf(stdout, "%p\t%d\n", ptr[1], *ptr[1]);
    ptr[2] = realloc(ptr[0], 9000);
    fprintf(stdout, "%p\t%d\n", ptr[2], *ptr[2]);
    
    free(ptr[1]);
    free(ptr[2]);
    free(ptr[0]);
    return 0;
}