#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
void __attribute__((constructor)) lib_init();
int search(void*);
#define list_size 9



void * map_list[list_size];
//when doing pointer arithmetic i'm starting at an int and based off of the size of the table changing the multiplier
int multiplier = 0;

int fd;


void lib_init(){
    fd = open ("/dev/zero", O_RDWR ) ;
    int * temp;
    //one for every size except greater than 1k.
    for(int i = 0; i < list_size - 1; ++i){
        //multiplier = pow(2, i + 3) / 4;
        map_list[i] = mmap ( NULL , 4096 , PROT_READ | PROT_WRITE , MAP_PRIVATE , fd , 0) ;
        temp = map_list[i];
        *temp = 0;
        temp += 2;
        temp = 54096;
        temp ++;
        temp = 22;
        temp = map_list[i];

        fprintf(stdout, "%p\t%d\n", temp, *temp);
        temp+=2;
        fprintf(stdout, "%p\t%d\n", temp, *temp);
        temp++;
        fprintf(stdout, "%p\t%d\n", temp, *temp);

    }
    
}

void * malloc(size_t size){

    if(size == 0) return NULL;
    void * page = mmap ( NULL , size , PROT_READ | PROT_WRITE , MAP_PRIVATE , fd , 0) ;

    

    return page;
}

void free(void * ptr){
    
}

void * calloc(size_t num, size_t size){
    
}

void * realloc(void * ptr, size_t size){
    
}
