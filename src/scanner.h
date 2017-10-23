//
// Created by Martin Bobčík on 10/23/17.
//

#ifndef SRC_SCANNER_H
#define SRC_SCANNER_H

typedef struct token{
    int token;
    char * data;
}token;

typedef struct buffer{
    int bSize;
    int actualSize;
    char * data;
}buffer;

token * getToken();
buffer * bInit(int size);
void bAdd(char c, buffer * buffer);
void bDispose(buffer * buffer);

enum tokenTypes {
    IDENTIFICATOR,
    INTEGER,
    DOUBLE,
    KEY_AS,
    KEY_ASC,
    KEY_DECLARE,
    KEY_DIM,
    KEY_DO,
    KEY_DOUBLE,
    KEY_ELSE,
    KEY_END,
    KEY_CHR,
    KEY_FUNCTION,
    KEY_IF,
    KEY_INPUT,
    KEY_INTEGER,
    KEY_LENGTH,
    KEY_LOOP,
    KEY_PRINT,
    KEY_RETURN,
    KEY_SCOPE,
    KEY_STRING,
    KEY_SUBSTR,
    KEY_THEN,
    KEY_WHILE,
    LITERAL,
    EOF,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    GREATER,
    LESSER,
    EQUAL,
    NOT_EQUAL,


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
};
#endif //SRC_SCANNER_H
