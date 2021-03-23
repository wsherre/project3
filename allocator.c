#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
void __attribute__((constructor)) lib_init();
int search(void*);
#define list_size 100000

typedef struct free_list{
    void * address;
    size_t size;
}free_list;


free_list list[list_size];
int list_index = 0;
int fd;


void lib_init(){
    for(int i = 0; i < list_size; ++i){
        list[i].address = NULL;
        list[i].size = 0;
    }
    fd = open ("/dev/zero", O_RDWR ) ;
}

void * malloc(size_t size){

    if(size == 0) return NULL;
    void * page = mmap ( NULL , size , PROT_READ | PROT_WRITE , MAP_PRIVATE , fd , 0) ;

    list[list_index].address = page;
    list[list_index].size = size;
    list_index++;

    return page;
}

void free(void * ptr){
    if(ptr == NULL) return;

    int index = search(ptr);
    if(index != -1){
        munmap(list[index].address, list[index].size);
        list[index].address = NULL;
        list[index].size = 0;
    }
}

void * calloc(size_t num, size_t size){
    void * ptr = malloc(num * size);
    memset(ptr, 0, num * size);
    return ptr;
}

void * realloc(void * ptr, size_t size){
    if(ptr == NULL){
        return malloc(size);
    }

    int index = search(ptr);

    void* newptr = malloc(size);
    memcpy(newptr, ptr, size);
    free(ptr);

    list[index].address = newptr;
    list[index].size = size;
    
    
    return newptr;
}

int search(void * ptr){
    if(ptr == NULL) return -1;

    for(int i = 0; i < list_size; ++i){
        if(list[i].address == ptr){
            return i;
        }
    }
    return -1;
}