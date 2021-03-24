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
        //fprintf(stdout, "%p\n", temp);
        unsigned int t = (unsigned int)(temp + 1) & 0x0fff;
        //fprintf(stdout, "%d\n", t);
        *temp = t;
        //fprintf(stdout, "%p\t%d\n", temp, *temp);
        
    }
    
}

void * malloc(size_t size){

    if(size == 0) return NULL;
    //this simple algorithm rounds up the size to the next highest power of 2
    unsigned v = size;
    if(size <= 1024){
        v--;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        v++;
        
        int i = log2(v) - 3;
        int* page_start = map_list[i];
        int offset = (int)page_start + 3;
        int* free_list = (int*) ((int)page_start & offset);

        int* return_ptr = free_list;

        int* next_ptr = return_ptr + v/4;
        *next_ptr = (int)(next_ptr++);
        *free_list = (int)next_ptr;
        return return_ptr;
    }
    return NULL;
}

void free(void * ptr){
    
}

void * calloc(size_t num, size_t size){
    void * page = malloc(num * size);
    memset(page, 0, num * size);
    return page;
}

void * realloc(void * ptr, size_t size){
    return NULL;
}
