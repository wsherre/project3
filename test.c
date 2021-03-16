#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(int argc, char** argv){

    int *ptr[5];
    for(int i = 0; i < 5; ++i){
        ptr[i] = malloc(5 * sizeof(int));
    } 
    for(int i = 0; i < 5; ++i){
        for(int k = 0; k < 5; ++k){
            printf("%d ", ptr[i][k]);
            ptr[i][k] = k;
        }
        printf("\n");
    }
    printf("\n");
    

    
    for(int i = 0; i < 5; ++i){
        for(int k = 0; k < 5; ++k){
            printf("%d ", ptr[i][k]);
        }
        printf("\n");
    }
    printf("\n");

    //ptr = realloc(ptr, 1 * sizeof(int));
    //ptr[1] = 6;

    //printf("%d\n", ptr[0]);
    
    for(int i = 0; i < 5; ++i){
        free(ptr[i]); 
    }
}