//
// Created by martin on 27.11.17.
//
#include "expression.h"

tokenTypes * expresionType;

const char precedentTable[16][16]=        // +	 -   *	 /  \    <   >  <=  >=  <>  ==   (	 )  id  lit  $
                            /* +  */      {{'>','>','<','<','<','>','>','>','>','>','>','<','>','<','<','>'},
                            /* -  */       {'>','>','<','<','<','>','>','>','>','>','>','<','>','<','<','>'},
                            /* *  */       {'>','>','>','>','>','>','>','>','>','>','>','<','>','<','<','>'},
                            /* /  */       {'>','>','>','>','>','>','>','>','>','>','>','<','>','<','<','>'},
                            /* \  */       {'>','>','<','<','>','>','>','>','>','>','>','<','>','<','<','>'},
                            /* <  */       {'<','<','<','<','<',' ',' ',' ',' ',' ',' ','<','>','<','<','>'},
                            /* >  */       {'<','<','<','<','<',' ',' ',' ',' ',' ',' ','<','>','<','<','>'},
                            /* <= */       {'<','<','<','<','<',' ',' ',' ',' ',' ',' ','<','>','<','<','>'},
                            /* >= */       {'<','<','<','<','<',' ',' ',' ',' ',' ',' ','<','>','<','<','>'},
                            /* <> */       {'<','<','<','<','<',' ',' ',' ',' ',' ',' ','<','>','<','<','>'},
                            /* == */       {'<','<','<','<','<',' ',' ',' ',' ',' ',' ','<','>','<','<','>'},
                            /* (  */       {'<','<','<','<','<','<','<','<','<','<','<','<','=','<','<',' '},
                            /* )  */       {'>','>','>','>','>','>','>','>','>','>','>',' ','>',' ',' ','>'},
                            /* id */       {'>','>','>','>','>','>','>','>','>','>','>',' ','>',' ',' ','>'},
                            /* lit*/       {'>','>','>','>','>','>','>','>','>','>','>',' ','>',' ',' ','>'},
                            /* $  */       {'<','<','<','<','<','<','<','<','<','<','<','<',' ','<','<',' '}};

int tokenTypeToPrecTableIndex(tokenTypes type) {
    switch (type) {

        case IDENTIFIER: return 13;
        case INTEGER:
            return 14;
        case DOUBLE:
            return 14;
        case STRING:
            return 14;
        case KEY_AS:break;
        case KEY_ASC:break;
        case KEY_DECLARE:break;
        case KEY_DIM:break;
        case KEY_DO:break;
        case KEY_DOUBLE:break;
        case KEY_ELSE:break;
        case KEY_END:break;
        case KEY_CHR:break;
        case KEY_FUNCTION:break;
        case KEY_IF:break;
        case KEY_INPUT:break;
        case KEY_INTEGER:break;
        case KEY_LENGTH:break;
        case KEY_LOOP:break;
        case KEY_PRINT:break;
        case KEY_RETURN:break;
        case KEY_SCOPE:break;
        case KEY_STRING:break;
        case KEY_SUBSTR:break;
        case KEY_THEN:break;
        case KEY_WHILE:break;
        case END_OF_FILE:break;
        case OPERATOR_PLUS: return 0;
        case OPERATOR_MINUS: return 1;
        case OPERATOR_MULTIPLY:
            return 2;
        case OPERATOR_DIVIDE:
            return 3;
        case OPERATOR_GREATER:
            return 6;
        case OPERATOR_GREATER_EQUAL:
            return 8;
        case OPERATOR_LESSER:
            return 5;
        case OPERATOR_LESSER_EQUAL:
            return 7;
        case OPERATOR_EQUAL:10;
        case OPERATOR_NOT_EQUAL:9;
        case OPERATOR_ASSIGN:break;
        case COMMA:break;
        case OPENING_BRACKET:
            return 11;
        case CLOSING_BRACKET:
            return 12;
        case SEMICOLON:break;
        case END_OF_LINE:break;
        case KEY_AND:break;
        case KEY_BOOLEAN:break;
        case KEY_CONTINUE:break;
        case KEY_ELSEIF:break;
        case KEY_EXIT:break;
        case KEY_FALSE:break;
        case KEY_FOR:break;
        case KEY_NEXT:break;
        case KEY_NOT:break;
        case KEY_OR:break;
        case KEY_SHARED:break;
        case KEY_STATIC:break;
        case KEY_TRUE:break;
    }

    return -1;
}



int expression(tokenTypes * tokenType){

    token * token = getToken();

    expresionType = tokenType;

    // je aktualni token zacatek expresion?
    if ( ! isTokenStartOfExpression(getToken())) {
        nextToken = getNextToken();
    }

    token = getToken();
    // Muze to byt prvni token expresion?
    if ( ! canBePartOfExpresion(token->tokenType)) {
        return SYNTAX_ERROR;
    }

    // Je to konec radku?
    if (token->tokenType == END_OF_LINE) {
        return SYNTAX_ERROR;
    }

    // sem si ulozime all tokeny ktere prindou v expresion
    tTokensList * listTokens;
    init_list(&listTokens);

    // Je to expresion?
    int ret;
    if ((ret = checkExpresion(listTokens)) != 0) {
        return ret;
    }

    // provedeni expresion
    // todo

    // vraceni ukonceni radku
    putTokenBack(nextToken);


    return 0;
}

bool isTokenStartOfExpression(token *token) {

    if (token == NULL) {
        return false;
    }
    switch (token->tokenType) {
        case OPERATOR_ASSIGN: // =
        case CLOSING_BRACKET: // )
        case KEY_AS: // key words
        case KEY_ASC:
        case KEY_DECLARE:
        case KEY_DIM:
        case KEY_DO:
        case KEY_DOUBLE:
        case KEY_ELSE: // 10
        case KEY_END:
        case KEY_CHR:
        case KEY_FUNCTION:
        case KEY_IF:
        case KEY_INPUT:
        case KEY_INTEGER:
        case KEY_LENGTH:
        case KEY_LOOP:
        case KEY_PRINT:
        case KEY_RETURN: // 20
        case KEY_SCOPE:
        case KEY_STRING:
        case KEY_SUBSTR:
        case KEY_THEN:
        case KEY_WHILE:
            return false;
        case IDENTIFIER:break;
        case INTEGER:break;
        case DOUBLE:break;
        case STRING:break;
        case END_OF_FILE:break;
        case OPERATOR_PLUS:break;
        case OPERATOR_MINUS:break;
        case OPERATOR_MULTIPLY:break;
        case OPERATOR_DIVIDE:break;
        case OPERATOR_GREATER:break;
        case OPERATOR_GREATER_EQUAL:break;
        case OPERATOR_LESSER:break;
        case OPERATOR_LESSER_EQUAL:break;
        case OPERATOR_EQUAL:break;
        case OPERATOR_NOT_EQUAL:break;
        case COMMA:break;
        case OPENING_BRACKET:break;
        case SEMICOLON:break;
        case END_OF_LINE:break;
        case KEY_AND:break;
        case KEY_BOOLEAN:break;
        case KEY_CONTINUE:break;
        case KEY_ELSEIF:break;
        case KEY_EXIT:break;
        case KEY_FALSE:break;
        case KEY_FOR:break;
        case KEY_NEXT:break;
        case KEY_NOT:break;
        case KEY_OR:break;
        case KEY_SHARED:break;
        case KEY_STATIC:break;
        case KEY_TRUE:break;
    }
    if (token->tokenType > 1000) {
        return false;
    }

    switch (token->tokenType) {
        case IDENTIFIER:
        case INTEGER:
        case DOUBLE:
        case STRING:
        case OPENING_BRACKET:
            return true;
        case KEY_AS:break;
        case KEY_ASC:break;
        case KEY_DECLARE:break;
        case KEY_DIM:break;
        case KEY_DO:break;
        case KEY_DOUBLE:break;
        case KEY_ELSE:break;
        case KEY_END:break;
        case KEY_CHR:break;
        case KEY_FUNCTION:break;
        case KEY_IF:break;
        case KEY_INPUT:break;
        case KEY_INTEGER:break;
        case KEY_LENGTH:break;
        case KEY_LOOP:break;
        case KEY_PRINT:break;
        case KEY_RETURN:break;
        case KEY_SCOPE:break;
        case KEY_STRING:break;
        case KEY_SUBSTR:break;
        case KEY_THEN:break;
        case KEY_WHILE:break;
        case END_OF_FILE:break;
        case OPERATOR_PLUS:break;
        case OPERATOR_MINUS:break;
        case OPERATOR_MULTIPLY:break;
        case OPERATOR_DIVIDE:break;
        case OPERATOR_GREATER:break;
        case OPERATOR_GREATER_EQUAL:break;
        case OPERATOR_LESSER:break;
        case OPERATOR_LESSER_EQUAL:break;
        case OPERATOR_EQUAL:break;
        case OPERATOR_NOT_EQUAL:break;
        case OPERATOR_ASSIGN:break;
        case COMMA:break;
        case CLOSING_BRACKET:break;
        case SEMICOLON:break;
        case END_OF_LINE:break;
        case KEY_AND:break;
        case KEY_BOOLEAN:break;
        case KEY_CONTINUE:break;
        case KEY_ELSEIF:break;
        case KEY_EXIT:break;
        case KEY_FALSE:break;
        case KEY_FOR:break;
        case KEY_NEXT:break;
        case KEY_NOT:break;
        case KEY_OR:break;
        case KEY_SHARED:break;
        case KEY_STATIC:break;
        case KEY_TRUE:break;
    }
    return false;
}

bool canBePartOfExpresion(tokenTypes type) {
    switch (type) {
        case IDENTIFIER:
            return true;
        case INTEGER:return true;
        case DOUBLE:return true;
        case STRING:return true;
        case KEY_AS:break;
        case KEY_ASC:break;
        case KEY_DECLARE:break;
        case KEY_DIM:break;
        case KEY_DO:break;
        case KEY_DOUBLE:break;
        case KEY_ELSE:break;
        case KEY_END:break;
        case KEY_CHR:break;
        case KEY_FUNCTION:break;
        case KEY_IF:break;
        case KEY_INPUT:break;
        case KEY_INTEGER:break;
        case KEY_LENGTH:break;
        case KEY_LOOP:break;
        case KEY_PRINT:break;
        case KEY_RETURN:break;
        case KEY_SCOPE:break;
        case KEY_STRING:break;
        case KEY_SUBSTR:break;
        case KEY_THEN:break;
        case KEY_WHILE:break;
        case END_OF_FILE:break;
        case OPERATOR_PLUS:
        case OPERATOR_MINUS:
        case OPERATOR_MULTIPLY:
        case OPERATOR_DIVIDE:
        case OPERATOR_GREATER:
        case OPERATOR_GREATER_EQUAL:
        case OPERATOR_LESSER:
        case OPERATOR_LESSER_EQUAL:
        case OPERATOR_EQUAL:
        case OPERATOR_NOT_EQUAL:
        case OPERATOR_ASSIGN:return true;
        case COMMA:break;
        case OPENING_BRACKET:
        case CLOSING_BRACKET:return true;
        case SEMICOLON:
            return true;
        case END_OF_LINE:
            return true;
        case KEY_AND:break;
        case KEY_BOOLEAN:break;
        case KEY_CONTINUE:break;
        case KEY_ELSEIF:break;
        case KEY_EXIT:break;
        case KEY_FALSE:break;
        case KEY_FOR:break;
        case KEY_NEXT:break;
        case KEY_NOT:break;
        case KEY_OR:break;
        case KEY_SHARED:break;
        case KEY_STATIC:break;
        case KEY_TRUE:break;
    }
    return false;
}

void init_list(tTokensList **tokenList) {

    *tokenList = malloc(sizeof(tTokensList));

    if (*tokenList == NULL) {
        // todo internal error
        return;
    }

    (*tokenList)->token = NULL;
    (*tokenList)->first = NULL;
    (*tokenList)->next = NULL;
}

void add_to_list(tTokensList *tokensList, token * token) {

    if (tokensList == NULL) {
        init_list(&tokensList);
    }


    // jo to prvni
    if (tokensList->token == NULL) {

        tokensList->token = token;
        tokensList->first = tokensList;
        tokensList->next = NULL;
    }
        // je to dalsi
    else {
        tTokensList * last = get_last_list(tokensList);

        if (last == NULL) {
            return;
        }

        tTokensList * tmp = malloc(sizeof(tTokensList));
        if (tmp == NULL) {
            // todo internal error
            return;
        }

        tmp->token = token;
        last->next = tmp;
        tmp->first = last->first;
    }
}

tTokensList *get_last_list(tTokensList *tokensList) {
    if (tokensList == NULL) {
        return NULL;
    }

    tTokensList * tmp = tokensList->first;

    while (tmp != NULL) {

        if (tmp->next == NULL) {
            return tmp;
        }
        tmp = tmp->next;
    }

    return NULL;
}

void remove_list(tTokensList *tokensList) {

    if (tokensList == NULL) {
        return;
    }

    tTokensList * tmp = tokensList->first;

    while (tmp != NULL) {

        tmp = tmp->next;
        tTokensList * tmpForFree = tmp;
        free(tmpForFree);
    }
}

int checkExpresion(tTokensList *tokensList) {

    if (tokensList == NULL) {
        return false;
    }

    // add prvni
    add_to_list(tokensList, getToken());

    token * token = getToken();


    // typ expresion
    tExpresionType type = noType_t;

    bool getSomeChars = false;
    int oppenBrackets = 0;
    bool existIdent = false;
    char * name;
    tItem * func;
    tItem * var;
    int ret;
    bool canEnd = false;

    while (1) {

        // Token, ktery neni soucasti expresion
        if ( ! canBePartOfExpresion(token->tokenType)) {
            return SYNTAX_ERROR;
        }

        // Zpracovani tokenu
        switch (token->tokenType) {

            case STRING: // string
                // kontrola typu
                if ((type == noType_t) || (type == string_t)) {
                    type = string_t;
                    canEnd = true; // z timto muze byt konec expresion
                }
                else {
                    return TYPE_SEMANTIC_ERROR;
                }

                break;
            case INTEGER: // integer
                // kontrola typu
                if ((type == noType_t) || (type == integer_t) || (type == double_t)) {
                    type = integer_t;
                    if (type == double_t) {
                        type = double_t;
                    }
                    canEnd = true; // z timto muze byt konec expresion
                }
                else {
                    return TYPE_SEMANTIC_ERROR;
                }

                break;

            case DOUBLE: // double
                // kontrola typu
                if ((type == noType_t) || (type == double_t) || (type == integer_t)) {
                    type = double_t;
                    canEnd = true; // z timto muze byt konec expresion
                }
                else {
                    return TYPE_SEMANTIC_ERROR;
                }

                break;

            case IDENTIFIER: // identifikator nebo funkce
                existIdent = false;
                // kontrola existence
                // If variable
                if (ht_isVarExist(contextSymTable, token->data) != HT_VAR_OK) {
                    existIdent = false;

                    // If function?
                    // next token je zavorka
                    nextToken = getNextToken();
                    if (nextToken == NULL) { // ukonceni drive
                        return SYNTAX_ERROR;
                    }

                    if (nextToken->tokenType != OPENING_BRACKET) {
                        // neni var ani neni fukci
                        // variable not declared
                        return VARIABLE_SEMANTIC_ERROR;
                    }

                    // je tam zavorka
                    // Existuje tato funkce?
                    if (ht_isFuncExist(contextSymTable, nextToken->data) != HT_FUNC_OK) {
                        // neni to ani funkce
                        return VARIABLE_SEMANTIC_ERROR; // neexistujici promnnena
                    }

                    name = token->data;


                    // funkce exituje je tam zavorka
                    if ((ret = functionExp(token->data)) != 0) {
                        return ret;
                    }

                    // je to funkce a je spravne volana
                    // podle toho jaky nam vraci datovy typ
                    func = ht_search(contextSymTable, name);

                    // Je definovana?
                    if ( ! ht_isFuncDefinedItem(func)) {
                        return VARIABLE_SEMANTIC_ERROR; // nedefinovana promenna
                    }

                    switch (func->dataType) {
                        case INTEGER:
                            // kontrola typu
                            if ((type == noType_t) || (type == integer_t) || (type == double_t)) {
                                type = integer_t;
                                if (type == double_t) {
                                    type = double_t;
                                }
                            }
                            else {
                                return TYPE_SEMANTIC_ERROR;
                            }
                            break;
                        case DOUBLE:
                            // kontrola typu
                            if ((type == noType_t) || (type == double_t) || (type == integer_t)) {
                                type = double_t;
                            }
                            else {
                                return TYPE_SEMANTIC_ERROR;
                            }
                            break;
                        case STRING:
                            if ((type == noType_t) || (type == string_t)) {
                                type = string_t;
                            }
                            else {
                                return TYPE_SEMANTIC_ERROR;
                            }
                            break;
                        default:
                            break;
                    }

                    // je to funkce a ma stravny datatype
                    // muzeme na dalsi
                }
                else if (ht_isVarExist(contextSymTable, token->data) == HT_VAR_OK) {
                    // variable existuje
                    // check datatype
                    var = ht_search(contextSymTable, token->data);

                    // Je definovana?
                    if ( ! ht_isVarDefinedItem(var)) {
                        return VARIABLE_SEMANTIC_ERROR; // nedefinovana promenna
                    }

                    switch (var->dataType) {
                        case INTEGER:
                            // kontrola typu
                            if ((type == noType_t) || (type == integer_t) || (type == double_t)) {
                                type = integer_t;
                                if (type == double_t) {
                                    type = double_t;
                                }
                            }
                            else {
                                return TYPE_SEMANTIC_ERROR;
                            }
                            break;
                        case DOUBLE:
                            // kontrola typu
                            if ((type == noType_t) || (type == double_t) || (type == integer_t)) {
                                type = double_t;
                            }
                            else {
                                return TYPE_SEMANTIC_ERROR;
                            }
                            break;
                        case STRING:
                            if ((type == noType_t) || (type == string_t)) {
                                type = string_t;
                            }
                            else {
                                return TYPE_SEMANTIC_ERROR;
                            }
                            break;
                        default:
                            break;
                    }

                    // je to ok variable
                }
                else {
                    // non existing variable
                    return VARIABLE_SEMANTIC_ERROR;
                }

                canEnd = true; // z timto muze byt konec expresion

            case OPENING_BRACKET: // (
                oppenBrackets++;
                break;
            case CLOSING_BRACKET: // )
                if (oppenBrackets == 0) {
                    return SYNTAX_ERROR;
                }
                oppenBrackets--;
                break;
            case END_OF_LINE: // konec expresion
            case SEMICOLON:
                if (canEnd) {
                    switch (type) {
                        case noType_t:
                            return SYNTAX_ERROR;
                        case string_t:
                            *expresionType = STRING;
                            break;
                        case integer_t:
                            *expresionType = INTEGER;
                            break;
                        case double_t:
                            *expresionType = DOUBLE;
                            break;
                    }
                    return 0; // ok muzeme ukoncit
                }
                else {
                    return SYNTAX_ERROR;
                }
            default:
                // ostatni matematicke symboly
                canEnd = false;
                break;
        }

        // ok nacteme dalsi token
        nextToken = getNextToken();
        token = getToken();
    }

    return 0;
}

int functionExp(char * functionName) {

    // Posuneme se na prvni param
    if (nextToken->tokenType == IDENTIFIER) { // Jeste nazev
        nextToken = getNextToken();
    }

    if (nextToken->tokenType == OPENING_BRACKET) { // Jeste (
        nextToken = getNextToken();
    }

    tFunctionParams  * params;
    st_init(&params);



    // nacitame dokud neni )
    while (1) {

        // Kontrola na konci funkce
        if (nextToken->tokenType == CLOSING_BRACKET) {
            // kontrola existence funkce s temito params

            // String
            if (ht_isFunction(&globalSymTable, functionName, STRING, params) == HT_FUNC_OK) {
                return 0;
            }
            else {
                // Integer
                if (ht_isFunction(&globalSymTable, functionName, INTEGER, params) == HT_FUNC_OK) {
                    return 0;
                }
                else {
                    // Double
                    if (ht_isFunction(&globalSymTable, functionName, DOUBLE, params) == HT_FUNC_OK) {
                        return 0;
                    }
                    else {
                        // neexistuje funkce s temito params
                        return SEMANTIC_ERROR;
                    }
                }
            }
        }

        // Nacteni param

        // datovy typ
        if ((nextToken->tokenType == KEY_DOUBLE)
            || (nextToken->tokenType == KEY_STRING)
            || (nextToken->tokenType == KEY_INTEGER)) {
            // dalsi musi byt param
            nextToken = getNextToken();

            // neni token
            if (nextToken == NULL) {
                return SYNTAX_ERROR; // neni dalsi token  func(double EOF
            }

            // neni prom
            if (nextToken->tokenType != IDENTIFIER) {
                return SYNTAX_ERROR; // neni prom func(double [!IDENTIFIER]
            }

            // pridani do param
            if ( ! st_addParam(params, nextToken->data, nextToken->tokenType)) {
                return INTERNAL_ERROR;
            }

            // je add param
        }
        else {
            // neni to param
            return SYNTAX_ERROR;
        }

        // Posun na dalsi token
        nextToken = getNextToken();
    }



    return SYNTAX_ERROR;
}

