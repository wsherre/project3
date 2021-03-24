#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(int argc, char** argv){

    int* ptr = NULL;
    ptr = malloc(5);
    *ptr = 16;
    fprintf(stdout, "%p\t%d\n", ptr, *ptr);
    return 0;
}