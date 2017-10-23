//
// Created by Martin Bobčík on 10/23/17.
//

#include <stdio.h>
#include <stdlib.h>
#include "error.h"

void throwError(int errorNumber, int line){
    char * errorMessage;
    switch (errorNumber){
        case LEXICAL_ERROR:
            errorMessage = "chyba v programu v rámci lexikální analýzy (chybná struktura aktuálního lexé-\n"
                    "mu).";
            break;
        case SYNTAX_ERROR:
            errorMessage = "chyba v programu v rámci syntaktické analýzy (chybná syntaxe programu).";
            break;
        case VARIABLE_SEMANTIC_ERROR:
            errorMessage = "sémantická chyba v programu – nedefinovaná funkce/promenná, pokus o redefi-\n"
            "nici funkce/promenné, atd. ";
            break;
        case TYPE_SEMANTIC_ERROR:
            errorMessage = "sémantická chyba typové kompatibility v aritmetických, řetezcových a rela čních\n"
            "výrazech, příp. špatný počet či typ parametru u volání funkce. ";
            break;
        case SEMANTIC_ERROR:
            errorMessage = "ostatní sémantické chyby.";
            break;
        case INTERNAL_ERROR:
            errorMessage = "interní chyba překladace tj. neovlivněná vstupním programem (např. chyba alokace\n"
            "paměti, atd.).";
            break;
    }

    fprintf(stderr, "Error number %d on line %d - %s\n", errorNumber,line,errorMessage);
    exit(errorNumber);
}
