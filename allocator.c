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
        *temp = (int)NULL;
        temp ++;
        fprintf(stdout, "%p\n", temp);
        int * test = temp + 1;
        fprintf(stdout, "%p\n", test);
        unsigned int t = (int)test | 0x0fff;
        fprintf(stdout, "%d\n", t);
        *temp = t;
        fprintf(stdout, "%p\t\t%p\t%d\n", temp, test, *test);
        
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
    return NULL;
}

void * realloc(void * ptr, size_t size){
    return NULL;
}
