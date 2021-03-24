#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(int argc, char** argv){

    int* ptr = NULL;
    for(int i = 0; i < 350; ++i){
        ptr = malloc(8);
        *ptr = i;
        fprintf(stdout, "%p\t%d\n", ptr, *ptr);
    }
    return 0;
}