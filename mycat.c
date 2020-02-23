
/*
Question1: mycat
Mudasir Hanif Shaikh (ms03831) and Kainat Abbasi (ka04051)
CS 2021, Habib University
Assignment 2, OS, Fall 2019
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //for close 
#include <fcntl.h> // for open
#include <string.h>
#include <errno.h>
/* References:
https://c-for-dummies.com/blog/?p=1758 
https://stackoverflow.com/questions/19472546/implicit-declaration-of-function-close */

extern int errno ;

int isNumbered(char* arg)
{
    if (!(strcmp(arg, "-n"))) return 1;
    return 0;
}

int main(int argc, char *argv[])
{ 
    int i;
    int line_count;
    FILE* file;
    size_t buffer = 120;
    char *line = (char *) malloc(buffer);
    int check = isNumbered(argv[1]);
    if (isatty(0) || argc > 1){
        for (i = 1; i < argc; i++) {  
            line_count = 1;
            //if isNumbered(argv[i]) continue;
            file = fopen(argv[i],"r"); //opens the file in readonly mode           
            if(file == NULL) { 
                fprintf(stderr, "mycat: %s %s\n", argv[i], strerror( errno ));
                continue;
            } 
            while ( getline ( &line, &buffer, file ) >= 0 )
            {
                if (check) printf("     %d %s", line_count, line);
                else printf("%s", line);
                line_count++;
            }
        fclose(file);                             
        }
    }
    else{
        char ch;
        while ( read(STDIN_FILENO, &ch, 1) > 0 )
            {
            printf("%c", ch);
            }
    }
    free(line);
    return 0;
}