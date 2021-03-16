#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(int argc, char** argv){

    int fd = open ("/dev/zero", O_RDWR ) ;

    void * page = mmap ( NULL , 4096 , PROT_READ | PROT_WRITE , MAP_PRIVATE , fd , 0) ;


    int *ptr;
    //ptr = malloc(sizeof(int));
    printf("%p    %lu \n", page, sizeof(page));

    //*ptr = 5;
    //printf("%d\n", *ptr);
    
    //free(ptr); 
}