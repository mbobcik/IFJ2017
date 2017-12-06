//
// Created by Martin Bobčík on 10/23/17.
//

#ifndef SRC_SCANNER_H
#define SRC_SCANNER_H

#include "functions.h"


#define CHARNUMBER_TO_INT(c) (c - 48)
#define TOKEN_DATA_SIZE 128
#define APOSTROPHE_ASCII_VALUE 39

token * getNextToken();
buffer * bInit(int size);
void bAdd(char c, buffer * buffer);
void bDispose(buffer * buffer);
void printBuffer(buffer * buffer);
//int isalphaMy(char c);
tokenTypes idOrKey(char * data);
void printToken(token * token);



/***
 *
 * Pro expresion - znovu zpracovani tokenu
 *
 */

/*
 * Ulozi posledni token znovu ke zpracovani
 *
 * @author Meluzin
 */
void putTokenBack(token * token);

/*
 * Zkontroluje jestli je nejaky token ke
 * znovu zpracovani
 *
 * @author Meluzin
 * @return 1 - true 0 - false
 */
int haveSomeTokenToProcess();

/*
 * Vrati token ke znovuzpracovani
 *
 * @author Meluzin
 * @return token *
 */
token * getTokenToProcess();

/*
 * Vrati aktualni token
 * nenacita dalsi
 *
 * @return token *
 */
token * getToken();


#endif //SRC_SCANNER_H
