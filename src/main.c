//
// Created by Martin Bobčík on 10/23/17.
//

#include "functions.h"



int main(int argc, char **argv) {

//    token * token = getNextToken();
//    printToken(token);
//    while (token->tokenType != END_OF_FILE) {
//        token = getNextToken();
//        printToken(token);
//    }

    input = stdin;

    if (DEBUG) {
        if (argc == 2) {
            input = fopen(argv[1], "r");
        }
    }


    int err = parse();
    if( err != 0){
        throwError(err,__LINE__);
    }
    return 0;
}