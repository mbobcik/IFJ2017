//
// Created by Martin Bobčík on 10/23/17.
//

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "error.h"

FILE * file;

int main(int argc, char * argv[]) {

    if(argc != 2){
        throwError(INTERNAL_ERROR,__LINE__);
    }
    if((file = fopen(argv[1], "r")) == NULL){
        throwError(INTERNAL_ERROR, __LINE__);
    }



    fclose(file);
    return 0;
}