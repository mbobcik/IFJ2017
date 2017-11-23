//
// Created by Martin Bobčík on 10/23/17.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "error.h"
#include "parser.h"


int main() {

//    token * token = getToken();
//    printToken(token);
//    while (token->tokenType != END_OF_FILE) {
//        token = getToken();
//        printToken(token);
//    }

    int err = parse();
    if( err != 0){
        throwError(err,__LINE__);
    }
    return 0;
}