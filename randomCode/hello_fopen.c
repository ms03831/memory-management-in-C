/*hello_fopen.c*/
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[]){

  char * input = malloc(120);
  printf(">>> ");
  scanf(input);
  printf(input);
  free(input);
}
