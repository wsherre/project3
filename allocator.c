#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
void __attribute__((constructor)) lib_init();
int search(void*);

typedef struct free_list{
    void * address;
    int size;
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

int search(void* ptr){
    for(int i = 0; i < 1000; ++i){
        if(list[i].address == ptr){
            return i;
        }
    }
    exit(0);
}