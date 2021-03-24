#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(int argc, char** argv){

    int* ptr = NULL;
    int* h = NULL;
    ptr = malloc(5);
    h = malloc(6);
    *ptr = 16;
    *h = 5;
    fprintf(stdout, "%p\t%d\n%p\t%d\n", ptr, *ptr, h, *h);
    return 0;
}