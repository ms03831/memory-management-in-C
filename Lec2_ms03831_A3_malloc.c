/*
Mudasir Hanif Shaikh (ms03831)
CS 2021, Habib University
Assignment 3, OS, Fall 2019
*/

#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "Lec2_ms03831_A3_malloc.h"

#define MEGABYTE 1024

my_node *head = NULL;
char* start = NULL;
const int MAGIC = 1234567;

int my_init(){
	if (head == NULL){
		void *ret = mmap(NULL, MEGABYTE, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
		if (ret == MAP_FAILED) return 0;
		else{
			head = (my_node *) ret;
			start = (char *) ret;
			head->size = MEGABYTE - sizeof(my_node); //1024*1024;
			head->next = 0;
			return 1;
		}
	}
	else {
		printf("%s\n", "Memoery already initialized");
		return 1;
	}
}

void *my_malloc(int size){
	if (head != NULL){
		my_node* current = head;
		my_node* temp = head;
		while (current->size < size + sizeof(my_node) && current->next != NULL){
			temp = current;
			current = current->next;
		}
		if (current->size < size + sizeof(my_node)){
			printf("%s\n", "MALLOC FAILED: not enough memory.");
			errno = ENOMEM;
			my_coalesce();
			return NULL;
		}
		else{
			if (current == head){
				head = (my_node*) (((char*)head) + size + sizeof(my_node));
				head->size = temp->size - size - sizeof(my_node);
				head->next = temp->next;
			}
			else{
				temp->next = (my_node*) ((char *) current + size + sizeof(my_node));
				temp->next->next = current->next;
				temp->next->size = current->size - size - sizeof(my_node);
			}
			current->size = size;
			current->next = (my_node*) &MAGIC;
			current++;
			return current;
		}
	}
	return NULL;
}

void my_free(void* ptr){
	if (head != NULL){
		my_node* freePtr = ((my_node *) ptr) - 1;
		if (freePtr->next == (my_node*) &MAGIC){
			 my_node* previousHead = head;
		     head = freePtr;
		     head->size = freePtr->size;
		     head->next = previousHead;
		}
		else{
			printf("%s\n", "The pointer passed to free is not valid");
			return;
		}
	}
}

void* my_calloc(int num, int size){
	if (head != NULL){
		my_node* t = my_malloc(num*size);
		if (t != NULL){
			char* temp = (char*) t;
			for (int i = 0; i < num*size; i++) {
				*temp = 0;
				temp++;
			}
			return t;
		}
		else{
			printf("%s\n", "my_calloc failed: not enough memory.");
			return NULL;
		}
	}
	return NULL;
}

void* my_realloc(void * ptr, int size){
	if (head != NULL){
		if (size < 0){
			printf("%s\n", "Please specify a valid size to reallocate");
			return NULL;
		}

		if (ptr == NULL){
			return my_malloc(size);
		}

		if (size == 0){ 
			my_free(ptr);
			return NULL;
		}
		else if (size > 0){
			my_node* temp = (my_node*) ptr;
			int previousSize = (temp - 1)->size;
			if ((temp - 1)->next == (my_node*)&MAGIC) {		
				my_node* newPtr = my_malloc(size);
				if (newPtr){
					
					char* previousMem = (char*) temp; 
					char* newMem = (char*) newPtr;
					int minimumSize = previousSize;
					if (size < minimumSize){
						minimumSize = size;
					}
					for (int i = 0; i < minimumSize; i++){
						*newMem = *previousMem;
						newMem++;
						previousMem++;
					}
					my_free(ptr);
					return newPtr;	
				}
				else{
					printf("reallocate failed \n");
					return NULL;
				}
			}
			else{
				printf("%s\n", "Pointer is not valid");
				return NULL;
			}
		}
		else{
			return NULL;
		}
	}
	return NULL;
}

void my_coalesce(){	
	if (head != NULL){
		my_node* starting_node = (my_node*)start;
		my_node* next_node_in_heap = (my_node*) (((char*) (starting_node + 1)) + starting_node->size);
		head = NULL;
		my_node* prev = NULL;
		while (next_node_in_heap < ((my_node*)(start + MEGABYTE))) {
			if (starting_node->next == (my_node*)&MAGIC){
				starting_node = (my_node*) (((char*) (starting_node + 1)) + starting_node->size);
				continue;
			}
			else{
				next_node_in_heap = (my_node*) (((char*) (starting_node + 1)) + starting_node->size);
				if (next_node_in_heap + 1 > ((my_node*)(start + MEGABYTE))) {
					return;
				}
				if (head == NULL){
					head = starting_node;
					head->size = starting_node->size;
					prev = head;
					prev->next = NULL;
					while (next_node_in_heap->next != (my_node*)&MAGIC) {
						head->size = head->size + next_node_in_heap->size + sizeof(my_node);
						next_node_in_heap = (my_node*) (((char*) (next_node_in_heap + 1)) + next_node_in_heap->size);
						starting_node = (my_node*) (((char*) (starting_node + 1)) + starting_node->size);
						if (starting_node + 1 > ((my_node*)(start + MEGABYTE))) {
							return;
						}
					}
				}
				else{
					prev->next = starting_node;
					prev = starting_node;
					while (next_node_in_heap->next != (my_node*)&MAGIC) {
						prev->next->size =  prev->next->size + next_node_in_heap->size + sizeof(my_node);
						next_node_in_heap = (my_node*) (((char*) (next_node_in_heap + 1)) + next_node_in_heap->size);
						starting_node = (my_node*) (((char*) (starting_node + 1)) + starting_node->size);
						if (starting_node + 1 > ((my_node*)(start + MEGABYTE))){
							return;
						}
					}
					
				}						
			}
		}
		prev->next = NULL;
	}
	return;
}

void my_showfreelist(){
	if (head != NULL){
		my_node* current = head;
		int no = 1;
		while (current != NULL){
			printf("%d: %d: %p\n", no, current->size, (void *) current);
			no++;
			current = current->next;
		}
	}
}

void my_uninit(){
	if (start != NULL){
		munmap(start, MEGABYTE);
		head = NULL;
		return;
	}
}

/*
my_node* current1 = head;
int no = 0;
while (current1 != NULL){
	no += current1->size;
	printf("%d %p %p %p \n", current1->size, current1, (char*)current1+current1->size+16, current1->next);
	current1 = current1->next;
}
printf("size %d\n", no); 
*/