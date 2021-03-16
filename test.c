#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(int argc, char** argv){

    int *ptr;
    ptr = malloc(5 * sizeof(int));
    for(int i = 0; i < 5; ++i){
        printf("%d\n", ptr[i]);
    }
    

    *ptr = 5;
    printf("%d\n", *ptr);

    ptr = realloc(ptr, 1 * sizeof(int));
    //ptr[1] = 6;

    printf("%d\n", ptr[0]);
    
    free(ptr); 
}