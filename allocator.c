#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
void __attribute__((constructor)) lib_init();
int search(void*);

typedef struct free_list{
    void * address;
    size_t size;
}free_list;


free_list list[1000];
int list_index = 0;


void lib_init(){
    for(int i = 0; i < 1000; ++i){
        list[i].address = NULL;
        list[i].size = 0;
    }
}

void * malloc(size_t size){
    int fd = open ("/dev/zero", O_RDWR ) ;

    void * page = mmap ( NULL , size , PROT_READ | PROT_WRITE , MAP_PRIVATE , fd , 0) ;

    list[list_index].address = page;
    list[list_index].size = size;
    list_index++;

    return page;
}

void free(void * ptr){
    int index = search(ptr);
    munmap(list[index].address, list[index].size);
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
    for(int i = 0; i < 1000; ++i){
        if(list[i].address == ptr){
            return i;
        }
    }
    exit(0);
}