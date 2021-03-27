#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(int argc, char** argv){

    int* ptr[100000];
    char* test;
    char letter = 'A';\

    ptr[0] = malloc(0);
    int i = 0;
    for(i = 4; i < 342; ++i){
        //int num = (rand() % 10000) + 4;
        ptr[i] = malloc(8);
        *ptr[i] = i;
        fprintf(stdout, "%p\t%d\n", ptr[i], *ptr[i]);

    }
    ptr[700] = malloc(8);
    printf("%p\n", ptr[700]);
    //for(int i = 4; i < 600; ++i){
        free(ptr[0]);
    //}

    test = malloc(15);
    for(int i = 0; i < 15; ++i){
        test[i] = letter;
        letter++;
    }
    for(int i = 0; i < 15; ++i){
        printf("%c\n", test[i]);
    }
    test = realloc(test, 7);
    for(int i = 0; i < 15; ++i){
        printf("%c\n", test[i]);
    }
    for(i = 4; i < 342; ++i){
        free(ptr[i]);
    }
    free(ptr[700]);
    free(test);
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