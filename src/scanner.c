//
// Created by Martin Bobčík on 10/23/17.
//

#include "scanner.h"
#include "error.h"


token * getToken(){

}

buffer * bInit(int size){
    buffer * newBuffer;
    if((newBuffer = (buffer *)malloc(sizeof(buffer))) == NULL) {
        throwError(INTERNAL_ERROR, __LINE__);
    }
    newBuffer->actualSize = 0;
    newBuffer->alocatedSize = size;
    if((newBuffer->data = malloc(size * sizeof(char))) == NULL){
        throwError(INTERNAL_ERROR,__LINE__);
    }
    newBuffer->data[0]="\0";
    return newBuffer;
}

void bAdd(char c, buffer * buffer){
    if(buffer == NULL){
        throwError(INTERNAL_ERROR,__LINE__);
    }


    if(buffer->actualSize + 1 > buffer->alocatedSize){

        buffer->alocatedSize = 2 * buffer->alocatedSize;

        buffer->data = realloc(buffer->data, buffer->alocatedSize * sizeof(char));

        if(buffer->data == NULL){
            throwError(INTERNAL_ERROR,__LINE__);
        }
    }


    buffer->data[buffer->actualSize++] = c;
    buffer->data[buffer->actualSize] == "\0";
}

void bDispose(buffer * buffer){
    if(buffer == NULL){
        throwError(INTERNAL_ERROR,__LINE__);
    }
    free(buffer->data);
    free(buffer);
}
void printBuffer(buffer * buffer){
    printf("Buffer->allocatedSize: %d\nBuffer->actualSize: %d\nBuffer->data: %s\n",buffer->alocatedSize,buffer->actualSize,buffer->data);
}