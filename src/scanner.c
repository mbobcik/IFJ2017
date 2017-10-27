//
// Created by Martin Bobčík on 10/23/17.
//

#include <string.h>
#include <ctype.h>
#include "scanner.h"
#include "error.h"


token * getToken(){
    token * newToken;
    if(!(newToken = (token *)malloc(sizeof(token)))){
        throwError(INTERNAL_ERROR,__LINE__);
    }
    if((newToken->data = malloc(sizeof(char) * TOKEN_DATA_SIZE))){
        throwError(INTERNAL_ERROR,__LINE__);
    }

    int identificatorFlag = 0;
    int integerFlag = 0;
    int dotFlag = 0;
    int EFlag = 0;
    int lineCommentFlag = 0;
    int possibleBlockCommentFlag = 0;
    int blockCommentFlag = 0;
    int possibleBlockUncommentFlag = 0;
    int assignFlag = 0;
    int greaterFlag = 0;
    int lesserFlag = 0;
    int toBeStringFlag = 0;

    buffer* buffer = bInit(2);
    char c;

    while ((c = getchar())) {
        if (c == EOF) {
            newToken->tokenType = END_OF_FILE;
            newToken->data = "EOF";
            bDispose(buffer);
            return newToken;
        } else if (identificatorFlag == 1) {              // Identificator

            if (isalphaMy(c) || isdigit(c) || c == '_') {   // alnum_ >> identificator
                bAdd(c, buffer);

            }
            // TODO find out how to end state, and ending conditions
        } else if(integerFlag == 1){
            if (isdigit(c)){
                bAdd(c,buffer);
            } else if (c == '.'){
                dotFlag = 1;
                bAdd(c,buffer);
            } else if (c =='e' ||
                       c == 'E'){
                EFlag = 1;
                bAdd(c,buffer);
            }
        } else if (isalphaMy(c) ||
                   c == '_'){       // start a-zA-Z> Identificator
            identificatorFlag = 1;
            bAdd(c,buffer);
        } else if (isdigit(c)){     // start number> int
            integerFlag = 1;
            bAdd(c,buffer);
        } else if (c == APOSTROPHE_ASCII_VALUE){ // start '\39' >> line_comment
            lineCommentFlag = 1;
        } else if (c == '/') {      // start '/'>> possible_block_comment
            possibleBlockCommentFlag = 1;
        } else if (c == ' ' ||
                   c == '\n' ||
                   c == '\t') {     //start whiteSpace >> start

        } else if (c == '='){       // start '=' >> Assign
            assignFlag = 1;
        } else if (c == '<'){       // start '<' >> Lesser
            lesserFlag = 1;
        } else if (c == '>'){       // start '>' >> Greater
            greaterFlag = 1;
        } else if (c == '!'){       // start '!' >> toBeString
            toBeStringFlag = 1;
        }
    }

}

int isalphaMy (char c){
    if ( c >='a' && c <= 'z')
        return 1;
    if (c >= 'A' && c <= 'Z')
        return 1;
    return 0;
}


buffer * bInit(int size){
    buffer * newBuffer;
    if(!(newBuffer = (buffer *)malloc(sizeof(buffer)))) {
        throwError(INTERNAL_ERROR, __LINE__);
    }
    newBuffer->actualSize = 0;
    newBuffer->alocatedSize = size;
    if(!(newBuffer->data = malloc(size * sizeof(char)))){
        throwError(INTERNAL_ERROR,__LINE__);
    }
    newBuffer->data[0]='\0';
    return newBuffer;
}

void bAdd(char c, buffer * buffer){
    if(!buffer){
        throwError(INTERNAL_ERROR,__LINE__);
    }


    if(buffer->actualSize + 1 > buffer->alocatedSize){

        buffer->alocatedSize = 2 * buffer->alocatedSize;

        buffer->data = realloc(buffer->data, buffer->alocatedSize * sizeof(char));

        if(!buffer->data){
            throwError(INTERNAL_ERROR,__LINE__);
        }
    }


    buffer->data[buffer->actualSize++] = c;
    buffer->data[buffer->actualSize] = '\0';
}

void bDispose(buffer * buffer){
    if(!buffer){
        throwError(INTERNAL_ERROR,__LINE__);
    }
    free(buffer->data);
    free(buffer);
}
void printBuffer(buffer * buffer){
    if(buffer){
        printf("Buffer->allocatedSize: %d\nBuffer->actualSize: %d\nBuffer->data: %s\n",buffer->alocatedSize,buffer->actualSize,buffer->data);
    }
}