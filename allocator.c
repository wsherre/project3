#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#define list_size 9
#define page_size 4096
#define max_block_size 1024

void __attribute__((constructor)) lib_init();
int search(void*);
void*new_map(int size);
void* big_map(int size);

void * map_list[list_size];
int fd;


void lib_init(){
    fd = open ("/dev/zero", O_RDWR ) ;
    for(int i = 0; i < list_size - 1; ++i){
        //map_list[i] = new_map(pow(2, i + 3));  
        map_list[i] = NULL;
    }
}

void * malloc(size_t size){

    if(size == 0) return NULL;
    //this simple algorithm rounds up the size to the next highest power of 2
    unsigned map_page_size = size;
    if(size <= max_block_size){
        if (size < 8) map_page_size = 7;
        map_page_size--;
        map_page_size |= map_page_size >> 1;
        map_page_size |= map_page_size >> 2;
        map_page_size |= map_page_size >> 4;
        map_page_size |= map_page_size >> 8;
        map_page_size |= map_page_size >> 16;
        map_page_size++;
        
        int i = log(map_page_size)/log(2) - 3;
        if(map_list[i] == NULL){
            map_list[i] = new_map(map_page_size);
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
            long * n_map = new_map(map_page_size);
            *next_page = (long)n_map;
            return malloc(size);
        }
        long* free_list = NULL;
        free_list = (long*) ((long)page_start | (long)offset);

        int* next_ptr = (int*) free_list;
        next_ptr += map_page_size/4;
        int ptr = 0;
        
        //*page_start += (map_page_size + 4);
        int next_start = (int)(next_ptr + 1) & 0xfff;
        if ((page_size  - next_start) < (map_page_size + 4)) 
            ptr = 0xffff;
        else 
            ptr = (long)(next_ptr + 1) & 0x0fff;
        
        *next_ptr = ptr;
        *(page_start + 4) = ptr;
        return (void*) free_list;
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
    int* int_page_start = (int*)temp;
    long* long_page_start = (long*)temp;
    long* original_next_page;
    int length, i, big = 0, size;
    if(*long_page_start < 0){
        big = 1;
        size = *long_page_start & 0x7fffffffffffffff;
        original_next_page = (long*)*(long_page_start + 1);
        i = 8;
    }else{
        original_next_page = (long*)*(long_page_start + 1);
        length = *(int_page_start + 1);
        *int_page_start -= (length + 4);
        i = log(length)/log(2) - 3;
        size = page_size;
    }


    if( ( *int_page_start == 20 || big) && original_next_page != NULL)
    {
        if((void*)long_page_start == map_list[i]){
            munmap(long_page_start, size);
            map_list[i] = (void*)original_next_page;
            return;
        }
        long* begin_of_page = (long*)map_list[i];
        long* begin_of_next_page = (long*)*(begin_of_page + 1);

        while(begin_of_next_page != long_page_start){ 
            begin_of_page = begin_of_next_page;       
            begin_of_next_page = (long*)*(begin_of_page + 1);
        }
        *(begin_of_page + 1) = (long)original_next_page;
        munmap(long_page_start, size);
    }
    else if( (*int_page_start == 20 || big) && original_next_page == NULL)
    {
        munmap(long_page_start, size);
        if(int_page_start == map_list[i]){
            map_list[i] = NULL;
            return;
        } else{
            long* begin_of_page = (long*)map_list[i];
            long* begin_of_next_page = (long*)*(begin_of_page + 1);

            while(begin_of_next_page != long_page_start){ 
                begin_of_page = begin_of_next_page;       
                begin_of_next_page = (long*)*(begin_of_page + 1);
            }
            *(begin_of_page + 1) = (int)NULL;
        }
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
    
    void* newptr = malloc(size);
    if(size < old_length)
        memcpy(newptr, ptr, size);
    else    
        memcpy(newptr, ptr, old_length);
    free(ptr);
    return newptr;

}

void* new_map(int map_page_size){
        int* temp;
        void * map = mmap ( NULL , page_size , PROT_READ | PROT_WRITE , MAP_PRIVATE , fd , 0);

        temp = map; 
        *temp = 20;

        temp++; 
        *temp = map_page_size;

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

