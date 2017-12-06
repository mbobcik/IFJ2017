//
// Created by Martin Bobčík on 10/23/17.
//

#ifndef SRC_ERROR_H
#define SRC_ERROR_H

#define LEXICAL_ERROR 1
#define SYNTAX_ERROR 2
#define VARIABLE_SEMANTIC_ERROR 3
#define TYPE_SEMANTIC_ERROR 4
#define SEMANTIC_ERROR 6
#define INTERNAL_ERROR 99

#include <stdio.h>
#include <stdlib.h>

void throwError(int errorNumber, int line);


#endif //SRC_ERROR_H
