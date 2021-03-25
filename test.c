#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(int argc, char** argv){

    int* ptr[679];
    for(int i = 0; i < 679; ++i){
        ptr[i] = malloc(8);
        *ptr[i] = i;
        fprintf(stdout, "%p\t%d\n", ptr[i], *ptr[i]);
    }
    for(int i = 339; i <= 679; ++i){
        free(ptr[i]);
    }
    return 0;
}