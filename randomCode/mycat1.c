
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
/*
int main(int argc, char *argv[])
{ 
    int i;
    int line_count = 1;
    FILE* file;
    size_t buffer = 120;
    char *line = (char *) malloc(buffer);
    int check, check1;
    if (isatty(0)) printf("%s", "yes");
    if (isatty(0) || argc > 1){
        for (i = 1; i < argc; i++) {  
            check = isNumbered(argv[i-1]);
            check1 = isNumbered(argv[i]); 
            if (check1 == 0 ){
                file = fopen(argv[i],"r"); //opens the file in readonly mode           
                if(file < 0) { 
                    perror("open"); 
                    return -1; 
                } 
                while ( getline ( &line, &buffer, file ) >= 0 )
                {
                    if (check) printf("     %d %s", line_count, line);
                    else printf("%s", line);
                    line_count++;
                }
            fclose(file);                             
            }
            else continue;
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
*/
/*
int main(int argc,char *argv[])
{ 
    int i;
    int line_count = 1;
    int file;
    char chr;
    
    for (i = 1; i < argc; i++) {               
        printf("%d", line_count);
        file = fopen(argv[i],); //opens the file in readonly mode           
        if(file < 0) { 
            perror("open"); 
            return -1; 
        } 
        write(1, &line_count, sizeof(line_count));
        
        while(read(file, &chr, 1)){        
            write(1, &chr, 1);
            if (chr == '\n') {
                line_count++;
                printf("%i", line_count);
                write(1, &line_count, sizeof(line_count));
            }
        }
    
        close(file);                             
    }
    return 0;
}
*/