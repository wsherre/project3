#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(int argc, char** argv){

    int *ptr;
    ptr = calloc(1, sizeof(int));
    printf("%d\n", *ptr);

    *ptr = 5;
    printf("%d\n", *ptr);

    ptr = realloc(ptr, 2 * sizeof(int));
    ptr[1] = 6;

    printf("%d %d\n", ptr[0], ptr[1]);
    
    free(ptr); 
}