//
//  head.c
//  
//
//  Created by Mudasir Hanif Shaikh on 08/10/2019.
//

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv []){
    
    FILE * stream = fopen(argv[1], "r");
    
    char *line = (char *) malloc(20);
    int c = 0;
    if(stream == NULL) {
        perror("Error opening file");
        return(-1);
    }
    else {
        while ( fgets ( line, 120, stream ) != NULL )
        {
            c++;
            printf("%s", line);
        }
    }
    fclose ( stream );
    if (c < 10) {
        printf("The file doesn't have 10 lines\n" );
    }
    free(line);
    return(0);
}

