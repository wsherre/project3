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
int return_i(int map_page_size);

void * map_list[list_size];
int fd;


void lib_init(){
    fd = open ("/dev/zero", O_RDWR ) ;
    for(int i = 0; i < list_size; ++i){
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
        
        int i = return_i(map_page_size);
        if(map_list[i] == NULL){
            map_list[i] = new_map(map_page_size);
        }
        short* short_page_start = map_list[i];
        long* long_page_start = map_list[i];
        long* next_page = (long*)*long_page_start;
        short* free_list = short_page_start + 5;
        short offset = *(free_list);

        while(offset == 0){
            if(next_page == NULL){
                map_list[i] = new_map(map_page_size);
                next_page = map_list[i];
                *next_page = (long)long_page_start;
                short_page_start = map_list[i];
                long_page_start = map_list[i];
                free_list = short_page_start + 5;
                offset = *(free_list);
            }else{
                short_page_start = (short*) next_page;
                long_page_start = next_page;
                next_page = (long*)*long_page_start;
                free_list = short_page_start + 5;
                offset = *(free_list);
            }
        }
        short * return_ptr = (short*)((long) short_page_start | (long)offset);
        *free_list = *(return_ptr + map_page_size/2);
        return return_ptr;
        
    }else
        return big_map(size);
    return NULL;
}

void free(void * ptr){
    if(ptr == NULL) return;
    long temp = (long)ptr & ~0xfff;
    short* short_page_start = (int*)temp;
    long* long_page_start = (long*)temp;
    long* original_next_page = NULL;
    int map_page_size;


    if(*long_page_start > 1024){
        map_page_size = *long_page_start;
        munmap(ptr, map_page_size);
        return;
    }else{
        map_page_size = *(short_page_start + 4);
        short* free_list = short_page_start + 5;
        short offset = *free_list;
        short* freed_ptr = (short*)ptr;
        freed_ptr += map_page_size/2;
        *freed_ptr = offset;
        *free_list = (short)((short)freed_ptr & 0xfff);
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


    if(*page_start > 1024){
        old_length = *page_start;
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
        short* temp;
        long* ptr;
        void * map = mmap ( NULL , page_size , PROT_READ | PROT_WRITE , MAP_PRIVATE | MAP_ANONYMOUS , -1 , 0);


        //0x...000
        temp = map;
        ptr = map;
        *ptr = 0;

        //0x...008
        temp+=4;
        *temp = map_page_size;

        //0x...00a = 0x...00c
        temp++;
        *temp = (short)(temp + 1) & 0x0fff;
        temp++;

        while( page_size - ((long) (temp + map_page_size/2 + 1) & 0xfff) > map_page_size + 2){
            temp += map_page_size/2;
            *temp = (short)(temp + 1) & 0x0fff;
            temp++;
        }
        temp += map_page_size/2;
        *temp = 0;
        temp = map;

        return map;
}

void * big_map(int size){
    long* temp = NULL;
    void * map = mmap ( NULL , size + 8, PROT_READ | PROT_WRITE , MAP_PRIVATE | MAP_ANONYMOUS, -1 , 0);
    temp = map;

    *temp = size + 8;
    temp++;
    return temp;
}


int return_i(int map_page_size){
    int i = 0;
    if(map_page_size == 8) i = 0;
    else if(map_page_size == 16) i = 1;
    else if(map_page_size == 32) i = 2;
    else if(map_page_size == 64) i = 3;
    else if(map_page_size == 128) i = 4;
    else if(map_page_size == 256) i = 5;
    else if(map_page_size == 512) i = 6;
    else if(map_page_size == 1024) i = 7;
    else i = 8;
    return i;
}
