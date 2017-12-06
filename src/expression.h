//
// Created by martin on 27.11.17.
//

#ifndef IFJ2017_EXPRESSION_H
#define IFJ2017_EXPRESSION_H

#include "functions.h"

int expression(tokenTypes * tokenType);


/**
 * Zkontroluje jestli je dany token zacatek expresion
 *
 * @author Meluzin
 * @param token
 * @return bool
 */
bool isTokenStartOfExpression(token *token);

/**
 * Muze byt token v exresion?
 *
 * @author Meluzin
 * @param type typ tokenu
 * @return
 */
bool canBePartOfExpresion(tokenTypes type);

/**
 * Prevod typu tokenu na index v prec tabulce
 *
 * @param type
 * @return
 */
int tokenTypeToPrecTableIndex(tokenTypes type);

/**
 * Zkontroluje expresion az do konce
 * jestli je ok
 *
 * @return int
 */
int checkExpresion(tTokensList * tokensList);


/**
 * Volani funkce z expresion
 * Predpoklada ze je jiz poslan nazev funkce
 * a jako dalsi token prijde prvni parametr nebo )
 *
 * @return chyba
 */
int functionExp(char * functionName);

///
/// Sznam
///

void init_list(tTokensList ** tokenList);

void remove_list(tTokensList * tokensList);

void add_to_list(tTokensList * tokensList, token * token);

tTokensList * get_last_list(tTokensList * tokensList);


#endif //IFJ2017_EXPRESSION_H

