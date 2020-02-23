//
//  cp.c
//  
//
//  Created by Mudasir Hanif Shaikh on 08/10/2019.
//

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv []){
    
    FILE * src = fopen(argv[1], "r");
    FILE * dst = fopen(argv[2], "w");
    
    char *line = (char *) malloc(120);
    if(src == NULL) {
        printf("Error opening source file");
        return(-1);
    }
    else {
        while ( fgets ( line, 120, src ) != NULL )
        {
            fputs(line, dst);
        }
    }
    free(line);
    fclose ( src );
    fclose ( dst );
    
    return(0);
}
