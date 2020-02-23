/*
Mudasir Hanif Shaikh (ms03831)
CS 2021, Habib University
Assignment 3, OS, Fall 2019
*/

#ifndef _Lec2_ms03831_A3_malloc_h
#define _Lec2_ms03831_A3_malloc_h

typedef struct node {
	int size;
	struct node *next;
} my_node;

int my_init();

void *my_malloc();

void my_free(void*);

void* my_calloc();

void* my_realloc(void *, int);

void my_coalesce();

void my_showfreelist();

void my_uninit();

#endif