//
// Created by Martin Bobčík on 10/23/17.
//

#include <string.h>
#include <ctype.h>
#include <limits.h>
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
            if (isalpha(c) || isdigit(c) || c == '_') {   // alnum_ >> identificator
                bAdd(tolower(c), buffer);

            } else if (isspace(c)){
                newToken->tokenType = idOrKey(buffer->data);
                strcpy(newToken->data,buffer->data);
                ungetc(c,stdin);
                return newToken;
            }else{
                throwError(LEXICAL_ERROR,__LINE__);
            }
        } else if(integerFlag == 1){
            if (isdigit(c)){        // integer number >> integer
                bAdd(c,buffer);
            } else if (c == '.'){   // integer '.' >> dotFlag
                dotFlag = 1;
                integerFlag = 0;
                bAdd(c,buffer);
            } else if (c =='e' ||
                       c == 'E'){       // integer e/E >> EFlag
                EFlag = 1;
                integerFlag = 0;
                bAdd(tolower(c),buffer);
            } else if(isspace(c)){ // ending in integer state
                newToken->tokenType = INTEGER;
                strcpy(newToken->data, buffer->data);
                ungetc(c, stdin);

                if( INT_MAX == ((int) strtol(newToken->data, (char **)NULL, 10)))
                    throwError(LEXICAL_ERROR,__LINE__);

                return newToken;
            } else {
                throwError(LEXICAL_ERROR,__LINE__); //<<<<<<<<< is this Right?
            }
        } else if (isalpha(c) ||
                   c == '_'){       // start a-zA-Z> Identificator
            identificatorFlag = 1;
            bAdd(tolower(c),buffer);
        } else if (isdigit(c)){     // start number >> int
            integerFlag = 1;
            bAdd(c,buffer);
        } else if (c == APOSTROPHE_ASCII_VALUE){ // start '\39' >> line_comment
            lineCommentFlag = 1;
        } else if (c == '/') {      // start '/'>> possible_block_comment
            possibleBlockCommentFlag = 1;
        } else if (isspace(c)){     //start whiteSpace >> start

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

/*
int isalphaMy (char c){
    if ( c >='a' && c <= 'z')
        return 1;
    if (c >= 'A' && c <= 'Z')
        return 1;
    return 0;
}
*/

tokenTypes idOrKey(char * data) {
    if (strcmp(data, "as")) {
        return KEY_AS;
    } else if (strcmp(data, "asc")) {
        return KEY_ASC;
    } else if (strcmp(data, "declare")) {
        return KEY_DECLARE;
    } else if (strcmp(data, "dim")) {
        return KEY_DIM;
    } else if (strcmp(data, "do")) {
        return KEY_DO;
    } else if (strcmp(data, "double")) {
        return KEY_DOUBLE;
    } else if (strcmp(data, "else")) {
        return KEY_ELSE;
    } else if (strcmp(data, "end")) {
        return KEY_END;
    } else if (strcmp(data, "chr")) {
        return KEY_CHR;
    } else if (strcmp(data, "function")) {
        return KEY_FUNCTION;
    } else if (strcmp(data, "if")) {
        return KEY_IF;
    } else if (strcmp(data, "input")) {
        return KEY_INPUT;
    } else if (strcmp(data, "integer")) {
        return KEY_INTEGER;
    } else if (strcmp(data, "length")) {
        return KEY_LENGTH;
    } else if (strcmp(data, "loop")) {
        return KEY_LOOP;
    } else if (strcmp(data, "print")) {
        return KEY_PRINT;
    } else if (strcmp(data, "return")) {
        return KEY_RETURN;
    } else if (strcmp(data, "scope")) {
        return KEY_SCOPE;
    } else if (strcmp(data, "string")) {
        return KEY_STRING;
    } else if (strcmp(data, "substr")) {
        return KEY_SUBSTR;
    } else if (strcmp(data, "then")) {
        return KEY_THEN;
    } else if (strcmp(data, "while")) {
        return KEY_WHILE;
    } else if (strcmp(data, "and")) {
        return KEY_AND;
    } else if (strcmp(data, "boolean")) {
        return KEY_BOOLEAN;
    } else if (strcmp(data, "continue")) {
        return KEY_CONTINUE;
    } else if (strcmp(data, "elseif")) {
        return KEY_ELSEIF;
    } else if (strcmp(data, "exit")) {
        return KEY_EXIT;
    } else if (strcmp(data, "false")) {
        return KEY_FALSE;
    } else if (strcmp(data, "for")) {
        return KEY_FOR;
    } else if (strcmp(data, "next")) {
        return KEY_NEXT;
    } else if (strcmp(data, "not")) {
        return KEY_NOT;
    } else if (strcmp(data, "or")) {
        return KEY_OR;
    } else if (strcmp(data, "shared")) {
        return KEY_SHARED;
    } else if (strcmp(data, "static")) {
        return KEY_STATIC;
    } else if (strcmp(data, "true")) {
        return KEY_TRUE;
    } else{
        return IDENTIFIER;
    }

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