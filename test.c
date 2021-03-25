#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(int argc, char** argv){

    int* ptr[350];
    for(int i = 0; i < 350; ++i){
        ptr[i] = malloc(8);
        *ptr[i] = i;
        fprintf(stdout, "%p\t%d\n", ptr, *ptr);
    }
    for(int i = 0; i < 350; ++i){
        free(ptr[i]);
    }
    return 0;
}