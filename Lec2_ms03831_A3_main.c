/*
Mudasir Hanif Shaikh (ms03831)
CS 2021, Habib University
Assignment 3, OS, Fall 2019
*/

#include <stdio.h>
#include "Lec2_ms03831_A3_malloc.h"

int main(int argc, char * argv[]) {
    my_init();
	my_showfreelist();
    void * ptr1 = my_malloc(100);
    printf("ptr1 malloc 100 \n");
    my_showfreelist();
    printf("\n");

    void * ptr2 = my_malloc(100);
    printf("ptr2 malloc 100 \n");
    my_showfreelist();
    printf("\n");

    void * ptr3 = my_malloc(100);
    printf("ptr3 malloc 100 \n");
    my_showfreelist();
    printf("\n");
    
    my_free(ptr3);
    printf("ptr3 free 100 \n");
    my_showfreelist();
    printf("\n");

    my_free(ptr2);
    printf("ptr2 free 100 \n");
    my_showfreelist();
    printf("\n");
    my_free(ptr1);
    printf("ptr1 free 100 \n");
    my_coalesce();
    my_showfreelist();
    printf("\n");
    
	
    void* ptr4 = my_malloc(50);
    printf("ptr4 malloc 50 \n");
    my_showfreelist();
    printf("\n");

    void* ptr5 = my_malloc(100);
    printf("ptr5 malloc 100 \n");
    my_showfreelist();
    printf("\n");

    void* ptr6 = my_calloc(5,5);
    printf("ptr6 calloc 5, 5 \n");
    my_showfreelist();
    printf("\n");

    char* ptr7 = (char*)ptr6;
    int i=0;
    for(i=0;i<((5*5));i++)
    {
        if (*ptr7 != 0)
        {
            printf("%s\n", "calloc not working");
        }
        ptr7++;
    }	
    ptr7 = (char*)ptr6;
    for(i=0;i<((5*5));i++)
    {
    	*ptr7 = i;
    	ptr7++;
    }

    void* ptr8 = my_realloc(ptr6, 30);
    char* ptr9 = (char*) ptr8;
    for(i=0;i<30;i++)
    {
        if (i < 25 && *ptr9 != i)
        {
            printf("%s\n", "my_realloc not working");
        }
        ptr9++;
    }	

    my_free(ptr8);
    my_free(ptr5);
    my_free(ptr4);
    my_showfreelist();
 	my_coalesce();
	my_showfreelist();
 	/*Unmap deletes the mapping*/
    printf("%s\n", "before uninit");
    my_showfreelist();
    my_uninit();
	printf("%s\n", "after");
    my_showfreelist();
}