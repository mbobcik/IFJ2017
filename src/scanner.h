//
// Created by Martin Bobčík on 10/23/17.
//

#ifndef SRC_SCANNER_H
#define SRC_SCANNER_H

#include <stdio.h>
#include <stdlib.h>

typedef struct buffer{
    int alocatedSize;
    int actualSize;
    char * data;
}buffer;

typedef enum FAStates {
    startFlag,
    identifierFlag,
    integerFlag,
    dotFlag,
    EFlag,
    lineCommentFlag,
    possibleBlockCommentFlag,
    blockCommentFlag,
    possibleBlockUncommentFlag,
    assignFlag,
    greaterFlag,
    lesserFlag,
    toBeStringFlag,
    stringFlag,
    doubleFlag,
} FAStates;

typedef enum tokenTypes {
    IDENTIFIER, // 0
    INTEGER,
    DOUBLE,     // todo
    STRING,     // todo
    KEY_AS,
    KEY_ASC,
    KEY_DECLARE,
    KEY_DIM,
    KEY_DO,
    KEY_DOUBLE,
    KEY_ELSE, // 10
    KEY_END,
    KEY_CHR,
    KEY_FUNCTION,
    KEY_IF,
    KEY_INPUT,
    KEY_INTEGER,
    KEY_LENGTH,
    KEY_LOOP,
    KEY_PRINT,
    KEY_RETURN, // 20
    KEY_SCOPE,
    KEY_STRING,
    KEY_SUBSTR,
    KEY_THEN,
    KEY_WHILE,
    // LITERAL, ??
    END_OF_FILE,            // fixme
    OPERATOR_PLUS,
    OPERATOR_MINUS,
    OPERATOR_MULTIPLY,
    OPERATOR_DIVIDE, // 30
    OPERATOR_GREATER,
    OPERATOR_GREATER_EQUAL,
    OPERATOR_LESSER,
    OPERATOR_LESSER_EQUAL,
    OPERATOR_EQUAL,
    OPERATOR_NOT_EQUAL,
    OPERATOR_ASSIGN,
    COMMA,                  // todo start
    OPENING_BRACKET,
    CLOSING_BRACKET, // 40
    SEMICOLON,              // todo end

    /////////Reserved keywords
    KEY_AND = 1001,
    KEY_BOOLEAN = 1002,
    KEY_CONTINUE = 1003,
    KEY_ELSEIF = 1004,
    KEY_EXIT = 1005,
    KEY_FALSE = 1006,
    KEY_FOR = 1007,
    KEY_NEXT = 1008,
    KEY_NOT = 1009,
    KEY_OR = 1010,
    KEY_SHARED = 1011,
    KEY_STATIC = 1012,
    KEY_TRUE = 1013
} tokenTypes;

typedef struct token{
    tokenTypes tokenType;
    char * data;
}token;

#define TOKEN_DATA_SIZE 128
#define APOSTROPHE_ASCII_VALUE 39

token * getToken();
buffer * bInit(int size);
void bAdd(char c, buffer * buffer);
void bDispose(buffer * buffer);
void printBuffer(buffer * buffer);
//int isalphaMy(char c);
tokenTypes idOrKey(char * data);
void printToken(token * token);

#endif //SRC_SCANNER_H
