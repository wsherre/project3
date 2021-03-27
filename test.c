#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(int argc, char** argv){

    int* ptr[100000];
    char* test;
    char* arr;
    char letter = 'A';
    arr = malloc(15);
    for(int i = 0; i < 15; ++i){
        arr[i] = letter;
        letter++;
    }
    

    ptr[0] = malloc(0);
    int i = 0;
    for(i = 4; i < 336; ++i){
        //int num = (rand() % 10000) + 4;
        ptr[i] = malloc(8);
        *ptr[i] = i;
        fprintf(stdout, "%p\t%d\n", ptr[i], *ptr[i]);

    }
    //for(int i = 4; i < 600; ++i){
        free(ptr[0]);
    //}

    test = malloc(15);
    for(int i = 0; i < 15; ++i){
        test[i] = arr[i];
    }
    fprintf(stdout, "%p\t%d\n", test, *test);
    test = realloc(test, 7);
    fprintf(stdout, "%p\t%d\n", test, *test);
    /*ptr[0] = malloc(1220);
    *ptr[0] = 1220;
    test = malloc(3000);
    *ptr[1] = 3000;
    ptr[2] = malloc(8000);
    *ptr[2] = 8000;
    ptr[3] = malloc(12000);
    *ptr[3] = 12000;
    fprintf(stdout, "%p\t%d\n", ptr[0], *ptr[0]);
    fprintf(stdout, "%p\t%d\n", ptr[1], *ptr[1]);
    fprintf(stdout, "%p\t%d\n", ptr[2], *ptr[2]);
    fprintf(stdout, "%p\t%d\n", ptr[3], *ptr[3]);


    free(ptr[1]);
    free(ptr[0]);
    free(ptr[3]);
    free(ptr[2]);*/
    

    
    return 0;
}