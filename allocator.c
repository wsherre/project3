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

        unsigned int v = 5;
        v--;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        v++;
        
    }
    
}

void * malloc(size_t size){

    if(size == 0) return NULL;

    if(size <= 8){
        int* ptr = map_list[0];

    }else if(size > 8 && size <= 16){
        int* ptr = map_list[0];

    }else if(size > 16 && size <= 32){
        int* ptr = map_list[0];

    }else if(size > 32 && size <= 64){
        int* ptr = map_list[0];

    }else if(size > 64 && size <= 128){
        int* ptr = map_list[0];

    }else if(size > 128 && size <= 256){
        int* ptr = map_list[0];

    }else if(size > 256 && size <= 512){
        int* ptr = map_list[0];

    }else if(size > 512 && size <= 1024){
        int* ptr = map_list[0];

    }else{

    }
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
