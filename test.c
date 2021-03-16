#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(int argc, char** argv){

    int *ptr;
    ptr = malloc(sizeof(int));
    printf("%p\n", ptr);

    *ptr = 5;
    printf("%d\n", *ptr);
    
    free(ptr); 
}