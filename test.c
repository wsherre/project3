#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(int argc, char** argv){

    int *ptr;
    ptr = calloc(5, sizeof(int));
    printf("%d\n", *ptr);

    *ptr = 5;
    printf("%d\n", *ptr);

    ptr = realloc(ptr, 1 * sizeof(int));
    //ptr[1] = 6;

    printf("%d\n", ptr[0]);
    
    free(ptr); 
}