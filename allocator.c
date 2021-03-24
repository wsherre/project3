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
        temp++;
        *temp = (int)NULL;
        temp+=2;
        //fprintf(stdout, "%p\n", temp);
        unsigned int t = (unsigned int)(temp + 1) & 0x00000fff;
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
        
        int i = log(v)/log(2) - 3;
        int* page_start = map_list[i];
        int* next_page = page_start + 1;
        while(next_page != NULL){
            page_start = next_page;
            next_page = page_start + 1;
        }
        unsigned int offset = ( unsigned int)*(page_start + 3);
        if(offset == 0xffff){
            *next_page = new_map();
            return malloc(size);
        }
        int* free_list = NULL;
        free_list = (int*) page_start;
        free_list = (int*) ((long)free_list | (long)offset);

        int* return_ptr = free_list;

        int* next_ptr = free_list + v/8;
        int ptr = 0;

        if(((int)(next_ptr + 1) & 0x0fff) > 0x0fff) 
            ptr = 0xffff;
        else 
            ptr = (long)(next_ptr + 1) & 0x0fff;
        
        *next_ptr = ptr;
        *(page_start + 3) = ptr;
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

void* new_map(){
        int* temp;
        void * map = mmap ( NULL , 4096 , PROT_READ | PROT_WRITE , MAP_PRIVATE , fd , 0) ;
        temp = map;
        *temp = 0;
        temp++;
        *temp = (int)NULL;
        temp+=2;
        //fprintf(stdout, "%p\n", temp);
        unsigned int t = (unsigned int)(temp + 1) & 0x00000fff;
        //fprintf(stdout, "%d\n", t);
        *temp = t;
        //fprintf(stdout, "%p\t%d\n", temp, *temp);

        return map;
}