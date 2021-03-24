#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(int argc, char** argv){

    int* ptr = NULL;
    int* h = NULL;

    int* i = NULL;
    ptr = malloc(5);
    h = malloc(6);
    i = malloc(7);
    *ptr = 16;
    *h = 5;
    *i = 22;
    fprintf(stdout, "%p\t%d\n%p\t%d\n%p\t%d\n", ptr, *ptr, h, *h, i, *i);
    return 0;
}