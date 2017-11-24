//
// Created by root on 11/23/17.
//

#ifndef EXAMPLE_FUNCTIONS_H
#define EXAMPLE_FUNCTIONS_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "scanner.h"
#include "symtable.h"


/*
 * Debug output
 *
 * @author Meluzin
 *
 * @param modulName
 * @param functionName
 * @param message   zprava na vypsani
 */
void debug_log(char * modulName, const char * functionName, char * message);


/*
 * Vypis tabulky na stdout
 *
 * @author Meluzin
 * @param table  tabulka symbolu
 */
void show_ht_table(ht_table * table, bool addPadding);

/*
 * Vypis tabulku pro funkci
 *
 * @author Meluzin
 * @param item
 */
void show_ht_funcTable(tItem * item);

/*
 * Vypis jednoho itemu
 *
 * @author Meluzin
 * @param item
 */
void show_ht_item(tItem * item, bool addPadding);

/*
 * Vypise seznam parametru
 *
 * @author Meluzin
 * @param list
 */
void show_st_list(tFunctionParams * list);

/*
 * Prevod interniho tokenu na debug vystup
 *
 * @param dataType  z tokenu
 */
const char * getDataTypeName(tokenTypes dataType);

#endif //EXAMPLE_FUNCTIONS_H
