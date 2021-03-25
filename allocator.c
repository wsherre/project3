#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#define list_size 9
#define page_size 4096

void __attribute__((constructor)) lib_init();
int search(void*);
void*new_map(int size);
void* big_map(int size);

void * map_list[list_size];
int fd;


void lib_init(){
    fd = open ("/dev/zero", O_RDWR ) ;
    for(int i = 0; i < list_size - 1; ++i){
        map_list[i] = NULL;  
    }
}

void * malloc(size_t size){

    if(size == 0) return NULL;
    //this simple algorithm rounds up the size to the next highest power of 2
    unsigned v = size;
    if(size <= 1024){
        if (size < 8) v = 7;
        v--;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        v++;
        
        int i = log(v)/log(2) - 3;
        if(map_list[i] == NULL){
            map_list[i] = new_map(v);
        }
        int* page_start = map_list[i];
        long* next_page = (long*)(page_start + 2);
        while(*next_page != (long)NULL){
            long* temp = next_page;
            page_start = (int*)*temp;
            next_page = (long*)(page_start + 2);
        }
        unsigned int offset = ( unsigned int)*(page_start + 4);
        if(offset == 0xffff){
            long * n_map = new_map(v);
            *next_page = (long)n_map;
            return malloc(size);
        }
        int* free_list = NULL;
        free_list = (int*) page_start;
        free_list = (int*) ((long)free_list | (long)offset);

        int* return_ptr = free_list;

        int* next_ptr = free_list + v/4;
        int ptr = 0;
        
        *page_start += (v + 4);
        int length = *page_start;
        if ((4096 - length) < v + 4) 
            ptr = 0xffff;
        else 
            ptr = (long)(next_ptr + 1) & 0x0fff;
        
        *next_ptr = ptr;
        *(page_start + 4) = ptr;
        return return_ptr;
    }else{
        if(map_list[list_size - 1] == NULL){
            map_list[list_size - 1] = big_map(size);
            long* page_start =  map_list[list_size - 1];
            page_start += 2;
            return page_start;
        }else{
            long* page_start =  map_list[list_size - 1];
            long* next_ptr = (long*)*(page_start + 1);

            while(next_ptr != NULL){
                page_start = next_ptr;
                next_ptr = (long*)*(next_ptr + 1);
            }
            long* new_block = (long*)big_map(size);
            *(page_start + 1) = (long)new_block;
            new_block +=2;
            return (void*)new_block;
        }
    }
}

void free(void * ptr){
    if(ptr == NULL) return;
    long temp = (long)ptr & ~0xfff;
    int* page_start = (int*)temp;
    long*big_start = (long*)temp;
    long* next_page;
    int length, i, big = 0, size;
    if(*big_start < 0){
        big = 1;
        size = *big_start & 0x7fffffffffffffff;
        next_page = big_start + 1;
        i = 8;
    }else{
        next_page = (long*)(page_start + 2);
        length = *(page_start + 1);
        *page_start -= (length + 4);
        i = log(length)/log(2) - 3;
        size = page_size;
    }


    if( (*page_start == 20 || big) && *next_page == (long)NULL)
    {
        munmap(page_start, size);
        if(page_start == map_list[i]){
            map_list[i] = NULL;
        } else{
            long* begin_of_page = (long*)map_list[i];
            long* next = (long*)*(begin_of_page + 1);

            while((int*)next != page_start){ 
                begin_of_page = next;       
                next = (long*)*(next + 1);
            }
            *(begin_of_page + 1) = (int)NULL;
        }
    }
    else if ( (*page_start == 20 || big) && *next_page != (long)NULL && (void*)page_start == map_list[i])
    {
        long* next = (long*)(page_start + 2);
        map_list[i] = (void*)*(next);
        munmap(page_start, size);
    }
    
    else if( ( *page_start == 20 || big) && *next_page != (long)NULL)
    {
        long* begin_of_page = (long*)map_list[i];
        long* next = (long*)*(begin_of_page + 1);

        while((int*)next != page_start){ 
            begin_of_page = next;       
            next = (long*)*(next + 1);
        }
        *(begin_of_page + 1) = *next_page;
        munmap(page_start, size);
    }
}

void * calloc(size_t num, size_t size){
    void * page = malloc(num * size);
    memset(page, 0, num * size);
    return page;
}

void * realloc(void * ptr, size_t size){
    if(ptr == NULL) return malloc(size);

    long temp = (long)ptr & ~0xfff;
    long* page_start = (long*)temp;
    int old_length = 0;


    if(*page_start < 0){
        old_length = *page_start & 0x7fffffffffffffff;
    }else{
        int* small_page = (int*)temp;
        old_length = *(small_page + 1);
    }
    if(size > old_length){
        void* newptr = malloc(size);
        memcpy(newptr, page_start + 2, old_length);
        free(page_start);
        return newptr;
    }
    return ptr;
}

void* new_map(int v){
        int* temp;
        void * map = mmap ( NULL , page_size , PROT_READ | PROT_WRITE , MAP_PRIVATE , fd , 0);

        temp = map; 
        *temp = 20;

        temp++; 
        *temp = v;

        temp++; 
        *temp = (int)NULL;

        temp+=2;
        unsigned int t = (unsigned int)(temp + 1) & 0x00000fff;
        *temp = t;

        return map;
}

void * big_map(int size){
    long* temp = NULL;
    void * map = mmap ( NULL , size + 16, PROT_READ | PROT_WRITE , MAP_PRIVATE , fd , 0);
    temp = map;

    *temp = (long)size;
    *temp |= 0x8000000000000000;
    temp++;
    *temp = (int)NULL;
    return map;
}

