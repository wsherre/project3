#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#define list_size 9
#define page_size 4096
#define max_block_size 1024
/*
small block headers are like so

|-----------------|-------------|-----------------|
| 8 byte next_ptr | 2 byte size | 2 byte free_list|
|-----------------|-------------|-----------------|

size is the size of the map, 8,16,32,64 etc
return the pointer by concatinating it with the page head

smabigll block headers are like so

|-----------------|
| 8 byte size     |
|-----------------|

dont really need anything else. just the size when needed to unmap
*/

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
    unsigned map_page_size = size;
    if(size <= max_block_size){
        if (size < 8) map_page_size = 7;
        //this simple algorithm rounds up the size to the next highest power of 2
        map_page_size--;
        map_page_size |= map_page_size >> 1;
        map_page_size |= map_page_size >> 2;
        map_page_size |= map_page_size >> 4;
        map_page_size |= map_page_size >> 8;
        map_page_size |= map_page_size >> 16;
        map_page_size++;
        
        int i = return_i(map_page_size);
        //if empty make it not empty
        if(map_list[i] == NULL){
            map_list[i] = new_map(map_page_size);
        }
        //one short and long page start. short for the pointer arithmetic
        short* short_page_start = map_list[i];
        long* long_page_start = map_list[i];
        long* next_page = (long*)*long_page_start;
        short* free_list = short_page_start + 5;
        short offset = *(free_list);

        //if offset is 0 then our map is full
        while(offset == 0){
            //if there isnt a new map, make one
            if(next_page == NULL){
                map_list[i] = new_map(map_page_size);
                //reset all the function variables
                next_page = map_list[i];
                *next_page = (long)long_page_start;
                short_page_start = map_list[i];
                long_page_start = map_list[i];
                free_list = short_page_start + 5;
                offset = *(free_list);
            }else{
                //set the function variables for the next map
                short_page_start = (short*) next_page;
                long_page_start = next_page;
                next_page = (long*)*long_page_start;
                free_list = short_page_start + 5;
                offset = *(free_list);
            }
        }
        //make the return pointer and update the free list
        short * return_ptr = (short*)((long) short_page_start | (long)offset);
        *free_list = *(return_ptr + map_page_size/2);
        return return_ptr;
        
    }else
        //big map return big pointer
        return big_map(size);
    return NULL;
}

void free(void * ptr){
    if(ptr == NULL) return;
    long temp = (long)ptr & ~0xfff;
    short* short_page_start = (short*)temp;
    long* long_page_start = ptr;
    long_page_start--;
    int map_page_size;


    if(*long_page_start < 0){
        map_page_size = *long_page_start & 0x7fffffffffffffff;
        munmap(ptr, map_page_size);
        return;
    }else{
        map_page_size = *(short_page_start + 4);
        short* free_list = short_page_start + 5;
        short offset = *free_list;
        short* freed_ptr = (short*)ptr;
        freed_ptr += map_page_size/2;
        *freed_ptr = offset;
        *free_list = (short)((short)ptr & 0xfff);
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
    long* long_page_start = ptr;
    long_page_start--;
    short* page_start = (short*)temp;
    int old_length = 0;


    if(*long_page_start < 0){
        old_length = (temp & 0x7fffffffffffffff);
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
    *temp |= 0x8000000000000000;
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
