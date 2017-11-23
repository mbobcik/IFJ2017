//
// Created by root on 11/23/17.
//
#include "functions.h"



void debug_log(char *modulName,const char *functionName, char *message) {
    printf("%s.%s: %s\n", modulName, functionName, message);
    fflush(stdout);
}
