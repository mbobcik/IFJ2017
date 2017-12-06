//
// Created by Martin Bobčík on 11/23/17.
//

#include "parser.h"

int parse() {

    // init symbol table
    ht_init(&globalSymTable);
    if (&globalSymTable == NULL) {
        throwError(INTERNAL_ERROR, __LINE__);
    }
    contextSymTable = &globalSymTable;

    // add  built-in functions
    tFunctionParams *paramStackLen;
    st_init(&paramStackLen);
    //////////////////////////////length
    if (!st_addParam(paramStackLen, "s", STRING)) {
        throwError(INTERNAL_ERROR, __LINE__);
    }
    if (ht_addFunctionWithParams(&globalSymTable, "length", INTEGER, paramStackLen) == NULL) {
        throwError(INTERNAL_ERROR, __LINE__);
    }

    //////////////////////////////substr
    tFunctionParams *paramStackSub;
    st_init(&paramStackSub);
    if (!st_addParam(paramStackSub, "s", STRING)) {
        throwError(INTERNAL_ERROR, __LINE__);
    }
    if (!st_addParam(paramStackSub, "i", INTEGER)) {
        throwError(INTERNAL_ERROR, __LINE__);
    }
    if (!st_addParam(paramStackSub, "n", INTEGER)) {
        throwError(INTERNAL_ERROR, __LINE__);
    }
    if (ht_addFunctionWithParams(&globalSymTable, "substr", STRING, paramStackSub) == NULL) {
        throwError(INTERNAL_ERROR, __LINE__);
    }
    //////////////////////////////asc
    tFunctionParams *paramStackAsc;
    st_init(&paramStackAsc);
    if (!st_addParam(paramStackAsc, "s", STRING)) {
        throwError(INTERNAL_ERROR, __LINE__);
    }
    if (!st_addParam(paramStackAsc, "i", INTEGER)) {
        throwError(INTERNAL_ERROR, __LINE__);
    }
    if (ht_addFunctionWithParams(&globalSymTable, "asc", INTEGER, paramStackAsc) == NULL) {
        throwError(INTERNAL_ERROR, __LINE__);
    }
    //////////////////////////////chr
    tFunctionParams *paramStackChr;
    st_init(&paramStackChr);
    if (!st_addParam(paramStackChr, "i", INTEGER)) {
        throwError(INTERNAL_ERROR, __LINE__);
    }
    if (ht_addFunctionWithParams(&globalSymTable, "chr", STRING, paramStackChr) == NULL) {
        throwError(INTERNAL_ERROR, __LINE__);
    }
    //add table for main function (scope)
    if (ht_addFunction(&globalSymTable, "scope", INTEGER) == NULL) {
        throwError(INTERNAL_ERROR, __LINE__);
    }

    ht_setFuncDefined(&globalSymTable, "length");
    ht_setFuncDefined(&globalSymTable, "substr");
    ht_setFuncDefined(&globalSymTable, "asc");
    ht_setFuncDefined(&globalSymTable, "chr");
    ht_setFuncDefined(&globalSymTable, "scope");
    if (DEBUG){
       // show_ht_table(&globalSymTable, false);
    }
    int err = prog();
    if(err != 0){
        return err;
    }

    //check if all functions are defined
    if(!ht_isAllDefined(&globalSymTable)){
        throwError(VARIABLE_SEMANTIC_ERROR,__LINE__);
    }

    if (DEBUG){
        show_ht_table(&globalSymTable, false);
    }
    return 0;
}
 /*
  * 1.  <prog> -> KEY_SCOPE END_OF_LINE <scope-st-list>
  * 2.  <prog> -> KEY_DECLARE KEY_FUNCTION IDENTIFIER OPENING_BRACKET <param-list> KEY_AS <data-type> END_OF_LINE <prog>
  * 2.  <prog> -> KEY_DECLARE KEY_FUNCTION IDENTIFIER OPENING_BRACKET <param-list> KEY_AS <data-type> END_OF_LINE <prog>
  * 4.  <prog> -> KEY_FUNCTION IDENTIFIER OPENING_BRACKET <param-list> KEY_AS <data-type> END_OF_LINE <fun-st-list>
  * 5.  <prog> -> END_OF_LINE <prog>
  */
int prog(){
     int err;
     nextToken = getNextToken();

     if( nextToken->tokenType == KEY_SCOPE){        // 1. <prog> -> KEY_SCOPE END_OF_LINE <scope-st-list>
         nextToken = getNextToken();
         if (nextToken->tokenType != END_OF_LINE){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
         }
         return scope_st_list();

     } else if (nextToken->tokenType == KEY_DECLARE) {   //2. <prog> -> KEY_DECLARE KEY_FUNCTION IDENTIFIER OPENING_BRACKET <param-list> KEY_AS <data-type> END_OF_LINE <prog>

         nextToken = getNextToken();
         if (nextToken->tokenType != KEY_FUNCTION) {
            throwError(SYNTAX_ERROR, __LINE__);
            return SYNTAX_ERROR;
         }

         nextToken = getNextToken();
         if (nextToken->tokenType != IDENTIFIER){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
         }
         char* functionName= nextToken->data;

         nextToken = getNextToken();
         if (nextToken->tokenType != OPENING_BRACKET){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
         }

         tFunctionParams * functionParams;
         st_init(&functionParams);
         err = param_list(functionParams);

         if(err != 0){
             throwError(err,__LINE__);
             return err;
         }

         nextToken = getNextToken();
         if(nextToken->tokenType != KEY_AS){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
         }

         int actualDataType = 0;
         err = data_type(&actualDataType); // in Dtype will be actual type
         if(err != 0){
             throwError(err,__LINE__);
             return err;
         }
         // check for redeclaration/override
         if ( (ht_isFuncExist(&globalSymTable, functionName) == HT_FUNC_OK)) {
             throwError(VARIABLE_SEMANTIC_ERROR,__LINE__);
         }
            //add function to symtable
         if (ht_addFunctionWithParams(&globalSymTable, functionName, actualDataType, functionParams) == NULL) {
             throwError(INTERNAL_ERROR,__LINE__);         }

         nextToken = getNextToken();
         if (nextToken->tokenType != END_OF_LINE){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
         }

         err = prog();
         if(err != 0) {
             throwError(err, __LINE__);
         }
         return err;
     } else if (nextToken->tokenType == KEY_FUNCTION){ // 4.  <prog> -> KEY_FUNCTION IDENTIFIER OPENING_BRACKET <param-list>  KEY_AS <data-type> END_OF_LINE <fun-st-list>

         nextToken = getNextToken();
         if(nextToken->tokenType != IDENTIFIER){
             throwError(SYNTAX_ERROR,__LINE__);
             return SYNTAX_ERROR;
         }
         char* functionName= nextToken->data;
         //maybe chech if declared

         //check redefinition

         //symtable->funcionName = nextToken->data;

         nextToken= getNextToken();
         if(nextToken->tokenType != OPENING_BRACKET){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
         }

         tFunctionParams *functionParams;
         st_init(&functionParams);
         err = param_list(functionParams);
         if(err != 0){
             throwError(err,__LINE__);
             return err;
         }
         //stuff params to symtable

         nextToken = getNextToken();
         if(nextToken->tokenType != KEY_AS){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
         }

         int actualDataType=0;
         err = data_type(&actualDataType);
         if(err != 0){
             throwError(err,__LINE__);
             return err;
         }
         //symtable->functionType = actualDataType ,,,, or check Datatype

         // check if exist function with same name
         if (ht_isFuncExist(&globalSymTable, functionName) == HT_FUNC_OK) {
             //check if exists function with exact same params
             if (ht_isFunction(&globalSymTable, functionName, actualDataType, functionParams) == HT_FUNC_OK){
                 //check if isDefined
                 if(ht_isFuncDefined(&globalSymTable,functionName)){ //if defined => redefinition => error
                     throwError(VARIABLE_SEMANTIC_ERROR,__LINE__);
                 } else {                                            // flag as defined
                     ht_setFuncDefined(&globalSymTable, functionName);
                 }
             } else { // exists with same name but different params/return value => override => error
                 throwError(VARIABLE_SEMANTIC_ERROR,__LINE__);
             }
         } else{                //no record of function with this name => add and flag as defined
             //add function to symtable
             if (ht_addFunctionWithParams(&globalSymTable, functionName, actualDataType, functionParams) == NULL) {
                 throwError(INTERNAL_ERROR,__LINE__);
             }
             ht_setFuncDefined(&globalSymTable, functionName);
         }
         actuallyParsedFunction = functionName;

         nextToken = getNextToken();
         if (nextToken->tokenType != END_OF_LINE){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
         }

         return fun_st_list();
     } else if (nextToken->tokenType == END_OF_LINE) {
         prog();
     } else{
        throwError(SYNTAX_ERROR,__LINE__);
        return SYNTAX_ERROR;
     }

     return 0;
}

/*
 * <fun-st-list>
 *  12. <fun-st-list> -> <fun-stat> <fun-st-list>
 *  14. <fun-st-list> -> KEY_END KEY_FUNCTION END_OF_LINE <prog>
 *      <fun-st-list> -> KEY_DIM IDENTIFIER KEY_AS <data-type> <assign> <fun-st-list>
 */
int fun_st_list(){
    nextToken= getNextToken();
    if(nextToken->tokenType == KEY_END) {    //<fun-st-list> -> KEY_END KEY_FUNCTION END_OF_LINE <prog>
        nextToken = getNextToken();
        if (nextToken->tokenType != KEY_FUNCTION) {
            throwError(SYNTAX_ERROR, __LINE__);
            return SYNTAX_ERROR;
        }

        nextToken= getNextToken();
        if (nextToken->tokenType != END_OF_LINE){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
        }
        return prog();
    } else if (nextToken->tokenType == KEY_DIM){    // <fun-st-list> -> KEY_DIM IDENTIFIER KEY_AS <data-type> <assign> <fun-st-list>
        nextToken= getNextToken();
        if (nextToken->tokenType != IDENTIFIER){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
        }
        char *varName = nextToken->data;
        //symtable.newVar.name = nextToken->data;
        nextToken= getNextToken();
        if (nextToken->tokenType != KEY_AS){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
        }

        int variableDataType = 0;
        int err = data_type(&variableDataType);
        if(err != 0){
            throwError(err,__LINE__);
            return err;
        }
        // newVariable.VarType = variableDataType
        // get function sym table
        ht_table *functionTable= ht_getTableFor(&globalSymTable,actuallyParsedFunction);
        if(functionTable == NULL){
            throwError(INTERNAL_ERROR,__LINE__);
        }
        contextSymTable = functionTable;

        //check if exists
        if (ht_isVarExist(functionTable, varName) == HT_VAR_OK) {
            throwError(VARIABLE_SEMANTIC_ERROR,__LINE__);
        }

        if (ht_addVariable(functionTable, varName, variableDataType) == NULL) {
            throwError(INTERNAL_ERROR,__LINE__);
        }

        tokenTypes expresionType;
        err = assign(&expresionType);
        if(err != 0){
            throwError(err,__LINE__);
            return err;
        }

        // Kotrola datoveho typu
        if (expresionType != variableDataType) {
            throwError(TYPE_SEMANTIC_ERROR, __LINE__); // pr. do int dava double
        }

        //maybe assign value to variable from assign() here
        if(variableDataType == DOUBLE){
            if (ht_setVarValueDouble(functionTable, varName, 0.0) != HT_VAR_OK) {
                throwError(INTERNAL_ERROR,__LINE__);
            }
        }else if (variableDataType == INTEGER){
            if (ht_setVarValueInt(functionTable, varName, 0) != HT_VAR_OK) {
                throwError(INTERNAL_ERROR,__LINE__);
            }
        }else {
            if (ht_setVarValueString(functionTable, varName, "") != HT_VAR_OK) {
                throwError(INTERNAL_ERROR,__LINE__);
            }
        }
        return fun_st_list();
    }

    int err = fun_stat();           //12. <fun-st-list> -> <fun-stat> <fun-st-list>
    if(err != 0){
        throwError(err,__LINE__);
        return err;
    }
    return fun_st_list();
}

/*
 * <end-prog>
 *  7.  <end-prog> -> END_OF_LINE <end-prog>
 *  8.  <end-prog> -> END_OF_FILE
 */
int end_prog(){
    nextToken = getNextToken();
    if(nextToken->tokenType == END_OF_FILE){
        return 0;
    }else if (nextToken->tokenType == END_OF_LINE){
        return end_prog();
    }
    throwError(SYNTAX_ERROR,__LINE__);
    return SYNTAX_ERROR;

}

/*
 * <scope-st-list>
 *  25. <scope-st-list> -> <scope-stat> <scope-st-list>
 *  28. <scope-st-list> -> KEY_END KEY_SCOPE <end-prog>
 *  29. <scope-st-list> -> KEY_DIM IDENTIFIER KEY_AS <data-type> <assign> <scope-st-list>
 */
int scope_st_list(){
   nextToken= getNextToken();
    if(nextToken->tokenType == KEY_END){    //28. <scope-st-list> -> KEY_END KEY_SCOPE <end-prog>
        nextToken= getNextToken();
        if(nextToken->tokenType != KEY_SCOPE){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
        }
        return end_prog();
    } else if (nextToken->tokenType == KEY_DIM){    // 29. <scope-st-list> -> KEY_DIM IDENTIFIER KEY_AS <data-type> <assign> <scope-st-list>
        nextToken= getNextToken();
        if (nextToken->tokenType != IDENTIFIER){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
        }

        //symtable.newVar.name = nextToken->data;
        char * varName=nextToken->data;
        nextToken= getNextToken();
        if (nextToken->tokenType != KEY_AS){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
        }

        int variableDataType = 0;
        int err = data_type(&variableDataType);
        if(err != 0){
            throwError(err,__LINE__);
            return err;
        }
        // newVariable.VarType = variableDataType

        ht_table *functionTable= ht_getTableFor(&globalSymTable,"scope");
        if(functionTable == NULL){
            throwError(INTERNAL_ERROR,__LINE__);
        }
        contextSymTable = functionTable;

        //check if exists
        if (ht_isVarExist(functionTable, varName) == HT_VAR_OK) {
            throwError(VARIABLE_SEMANTIC_ERROR,__LINE__);
        }

        if (ht_addVariable(functionTable, varName, variableDataType) == NULL) {
            throwError(INTERNAL_ERROR,__LINE__);
        }

        tokenTypes typeOfExpresion;
        err = assign(&typeOfExpresion);
        if(err != 0){
            throwError(err,__LINE__);
            return err;
        }

        // Kontrola jestli sedi datove typy
        tokenTypes varType = ht_getDataType(functionTable, varName);
        if (varType != typeOfExpresion) {
            if ((varType == DOUBLE) && (typeOfExpresion == INTEGER)) {
                // taky ok, jen se to pretypuje
            }
            else {
                throwError(TYPE_SEMANTIC_ERROR, __LINE__);
            }
        }


        //maybe assign value to variable from assign() here
        if(variableDataType == DOUBLE){
            if (ht_setVarValueDouble(functionTable, varName, 0.0) != HT_VAR_OK) {
                throwError(INTERNAL_ERROR,__LINE__);
            }
        }else if (variableDataType == INTEGER){
            if (ht_setVarValueInt(functionTable, varName, 0) != HT_VAR_OK) {
                throwError(INTERNAL_ERROR,__LINE__);
            }
        }else {
            if (ht_setVarValueString(functionTable, varName, "") != HT_VAR_OK) {
                throwError(INTERNAL_ERROR,__LINE__);
            }
        }

        return scope_st_list();
    }

    int err = scope_stat();             // 25. <scope-st-list> -> <scope-stat> <scope-st-list>
    if(err != 0){
        throwError(err,__LINE__);
        return err;
    }

    return scope_st_list();
}

/*
 * <scope-stat>
 *  29. <scope-stat> -> END_OF_LINE
 *  31. <scope-stat> -> IDENTIFIER OPERATOR_ASSIGN <expression> END_OF_LINE
 *  32. <scope-stat> -> KEY_INPUT IDENTIFIER END_OF_LINE
 *  33. <scope-stat> -> KEY_PRINT <print-list> END_OF_LINE
 *  34. <scope-stat> -> KEY_IF <expression> KEY_THEN END_OF_LINE <scope-if-stat-list> <scope-else-stat-list> END_OF_LINE
 *  35. <scope-stat> -> KEY_DO KEY_WHILE <expression> END_OF_LINE <scope-while-stat-list> END_OF_LINE
*/
int scope_stat() {

    if (nextToken->tokenType == END_OF_LINE) { // 29. <scope-stat> -> END_OF_LINE
        return 0;
    } else if (nextToken->tokenType == IDENTIFIER) { //  31. <scope-stat> -> IDENTIFIER OPERATOR_ASSIGN <expression> END_OF_LINE

        // check if in table
        ht_table *functionTable= ht_getTableFor(&globalSymTable,"scope");
        if (ht_isVarExist(functionTable, nextToken->data) != HT_VAR_OK) {
            throwError(VARIABLE_SEMANTIC_ERROR,__LINE__);
        }

        contextSymTable = functionTable;
        char * varName = nextToken->data;

        nextToken = getNextToken();
        if (nextToken->tokenType != OPERATOR_ASSIGN) {
            throwError(SYNTAX_ERROR, __LINE__);
            return SYNTAX_ERROR;
        }

        // if ID is_function then IDENTIFIER OPERATOR_ASSIGN IDENTIFIER OPENING_BRACKET <param-id-list> END_OF_LINE                 //todo
        //else expression
        tokenTypes expresionType;
        int err = 0;
        err = expression(&expresionType);
        if (err != 0) {
            throwError(err, __LINE__);
            return err;
        }

        // Kontrola jestli sedi datove typy
        tokenTypes varType = ht_getDataType(functionTable, varName);
        if (varType != expresionType) {
            if ((varType == DOUBLE) && (expresionType == INTEGER)) {
                // taky ok, jen se to pretypuje
            }
            else {
                throwError(TYPE_SEMANTIC_ERROR, __LINE__);
            }
        }

        nextToken = getNextToken();
        if (nextToken->tokenType != END_OF_LINE) {
            throwError(SYNTAX_ERROR, __LINE__);
            return SYNTAX_ERROR;
        }
        return 0;
    } else if (nextToken->tokenType == KEY_INPUT) { //  32. <scope-stat> -> KEY_INPUT IDENTIFIER END_OF_LINE
        nextToken = getNextToken();
        if (nextToken->tokenType != IDENTIFIER) {
            throwError(SYNTAX_ERROR, __LINE__);
            return SYNTAX_ERROR;
        }

        // check if in table
        ht_table *functionTable= ht_getTableFor(&globalSymTable,"scope");
        if (ht_isVarExist(functionTable, nextToken->data) != HT_VAR_OK) {
            throwError(VARIABLE_SEMANTIC_ERROR,__LINE__);
        }

        //symtable.checkVarName();
        nextToken = getNextToken();
        if (nextToken->tokenType != END_OF_LINE) {
            throwError(SYNTAX_ERROR, __LINE__);
            return SYNTAX_ERROR;
        }
        return 0;
    } else if (nextToken->tokenType == KEY_PRINT) { // 33. <scope-stat> -> KEY_PRINT <print-list> END_OF_LINE

        ht_table *functionTable= ht_getTableFor(&globalSymTable,"scope");
        contextSymTable = functionTable;

        int err = 0;
        err = LLrule_print_list();
        if (err != 0) {
            throwError(err, __LINE__);
            return err;
        }

        return 0;
    } else if (nextToken->tokenType == KEY_IF) { // 34. <scope-stat> -> KEY_IF <expression> KEY_THEN END_OF_LINE <scope-if-stat-list> <scope-else-stat-list> END_OF_LINE

        // check if in table
        ht_table *functionTable= ht_getTableFor(&globalSymTable,"scope");
        contextSymTable = functionTable;

        int err = 0;
        tokenTypes expresionType;
        err = expression(&expresionType);
        if (err != 0) {
            throwError(err, __LINE__);
            return err;
        }

        nextToken = getNextToken();
        if (nextToken->tokenType != KEY_THEN) {
            throwError(SYNTAX_ERROR, __LINE__);
            return SYNTAX_ERROR;
        }

        nextToken = getNextToken();
        if (nextToken->tokenType != END_OF_LINE) {
            throwError(SYNTAX_ERROR, __LINE__);
            return SYNTAX_ERROR;
        }

        err = scope_if_stat_list();
        if (err != 0) {
            throwError(err, __LINE__);
            return err;
        }

        err = scope_else_stat_list();
        if (err != 0) {
            throwError(err, __LINE__);
            return err;
        }
        nextToken = getNextToken();
        if (nextToken->tokenType != END_OF_LINE) {
            throwError(SYNTAX_ERROR, __LINE__);
            return SYNTAX_ERROR;
        }

        return 0;
    } else if (nextToken->tokenType == KEY_DO) { // 35. <scope-stat> -> KEY_DO KEY_WHILE <expression> END_OF_LINE <scope-while-stat-list> END_OF_LINE
        nextToken = getNextToken();
        if (nextToken->tokenType != KEY_WHILE) {
            throwError(SYNTAX_ERROR, __LINE__);
            return SYNTAX_ERROR;
        }

        ht_table *functionTable= ht_getTableFor(&globalSymTable,"scope");
        contextSymTable = functionTable;

        int err = 0;
        tokenTypes expresionType;
        err = expression(&expresionType);
        if (err != 0) {
            throwError(err, __LINE__);
            return err;
        }

        nextToken = getNextToken();
        if (nextToken->tokenType != END_OF_LINE) {
            throwError(SYNTAX_ERROR, __LINE__);
            return SYNTAX_ERROR;
        }
        err = scope_while_stat_list();
        if (err != 0) {
            throwError(err, __LINE__);
            return err;
        }
        nextToken = getNextToken();
        if (nextToken->tokenType != END_OF_LINE) {
            throwError(SYNTAX_ERROR, __LINE__);
            return SYNTAX_ERROR;
        }
        return 0;
    } else {
        throwError(SYNTAX_ERROR, __LINE__);
        return SYNTAX_ERROR;
    }
}

/*
 * <fun-stat>
 *  15. <fun-stat> -> END_OF_LINE
 *  17. <fun-stat> -> IDENTIFIER OPERATOR_ASSIGN <expression> END_OF_LINE
 *  18. <fun-stat> -> KEY_INPUT IDENTIFIER END_OF_LINE
 *  19. <fun-stat> -> KEY_PRINT <print-list>
 *  20. <fun-stat> -> KEY_IF <expression> KEY_THEN END_OF_LINE <fun-if-stat-list> <fun-else-stat-list> END_OF_LINE
 *  21. <fun-stat> -> KEY_DO KEY_WHILE <expression> END_OF_LINE <fun-while-stat-list> END_OF_LINE
 *  23. <fun-stat> -> IDENTIFIER OPERATOR_ASSIGN IDENTIFIER OPENING_BRACKET <param-id-list> END_OF_LINE //mee too
 *  24. <fun-stat> -> KEY_RETURN <expression> END_OF_LINE
 */
int fun_stat(){
    if(nextToken->tokenType == END_OF_LINE){
        return 0;
    } else if (nextToken->tokenType == IDENTIFIER){ // 17. <fun-stat> -> IDENTIFIER OPERATOR_ASSIGN <expression> END_OF_LINE

        //check if in table
        ht_table *functionTable= ht_getTableFor(&globalSymTable,actuallyParsedFunction);
        if (ht_isVarExist(functionTable, nextToken->data) != HT_VAR_OK) {
            throwError(VARIABLE_SEMANTIC_ERROR,__LINE__);
        }

        contextSymTable = functionTable;
        char * varName = nextToken->data;

        nextToken = getNextToken();

        if (nextToken->tokenType != OPERATOR_ASSIGN) {
            throwError(SYNTAX_ERROR, __LINE__);
            return SYNTAX_ERROR;
        }

        // if ID is_function then IDENTIFIER OPERATOR_ASSIGN IDENTIFIER OPENING_BRACKET <param-id-list> END_OF_LINE                 //todo
        //else expression
        int err = 0;
        tokenTypes expresionType;
        err = expression(&expresionType);
        if (err != 0) {
            throwError(err, __LINE__);
            return err;
        }

        // Kontrola jestli sedi datove typy
        tokenTypes varType = ht_getDataType(functionTable, varName);
        if (varType != expresionType) {
            if ((varType == DOUBLE) && (expresionType == INTEGER)) {
                // taky ok, jen se to pretypuje
            }
            else {
                throwError(TYPE_SEMANTIC_ERROR, __LINE__);
            }
        }

        nextToken = getNextToken();
        if (nextToken->tokenType != END_OF_LINE) {
            throwError(SYNTAX_ERROR, __LINE__);
            return SYNTAX_ERROR;
        }
        return 0;
    } else if (nextToken->tokenType == KEY_INPUT){   // 18. <fun-stat> -> KEY_INPUT IDENTIFIER END_OF_LINE
        nextToken = getNextToken();
        if (nextToken->tokenType != IDENTIFIER) {
            throwError(SYNTAX_ERROR, __LINE__);
            return SYNTAX_ERROR;
        }

        ht_table *functionTable= ht_getTableFor(&globalSymTable,actuallyParsedFunction);
        if (ht_isVarExist(functionTable, nextToken->data) != HT_VAR_OK) {
            throwError(VARIABLE_SEMANTIC_ERROR,__LINE__);
        }

        contextSymTable = functionTable;

        //symtable.checkVarName();
        nextToken = getNextToken();
        if (nextToken->tokenType != END_OF_LINE) {
            throwError(SYNTAX_ERROR, __LINE__);
            return SYNTAX_ERROR;
        }
        return 0;
    } else if (nextToken->tokenType == KEY_PRINT){  // 19. <fun-stat> -> KEY_PRINT <print-list>

        ht_table *functionTable= ht_getTableFor(&globalSymTable,actuallyParsedFunction);
        contextSymTable = functionTable;

        int err = 0;
        err = LLrule_print_list();
        if (err != 0) {
            throwError(err, __LINE__);
            return err;
        }

        return 0;
    } else if (nextToken->tokenType == KEY_IF){     // 20. <fun-stat> -> KEY_IF <expression> KEY_THEN END_OF_LINE <fun-if-stat-list> <fun-else-stat-list> END_OF_LINE

        ht_table *functionTable= ht_getTableFor(&globalSymTable,actuallyParsedFunction);
        contextSymTable = functionTable;

        int err = 0;
        tokenTypes expresionType;
        err = expression(&expresionType);
        if (err != 0) {
            throwError(err, __LINE__);
            return err;
        }
        nextToken = getNextToken();
        if (nextToken->tokenType != KEY_THEN) {
            throwError(SYNTAX_ERROR, __LINE__);
            return SYNTAX_ERROR;
        }
        nextToken = getNextToken();
        if (nextToken->tokenType != END_OF_LINE) {
            throwError(SYNTAX_ERROR, __LINE__);
            return SYNTAX_ERROR;
        }

        err = fun_if_stat_list();
        if (err != 0) {
            throwError(err, __LINE__);
            return err;
        }

        err = fun_else_stat_list();
        if (err != 0) {
            throwError(err, __LINE__);
            return err;
        }

        nextToken = getNextToken();
        if (nextToken->tokenType != END_OF_LINE) {
            throwError(SYNTAX_ERROR, __LINE__);
            return SYNTAX_ERROR;
        }

        return 0;
    } else if (nextToken->tokenType == KEY_DO){     // 21. <fun-stat> -> KEY_DO KEY_WHILE <expression> END_OF_LINE <fun-while-stat-list> END_OF_LINE
        nextToken = getNextToken();
        if (nextToken->tokenType != KEY_WHILE) {
            throwError(SYNTAX_ERROR, __LINE__);
            return SYNTAX_ERROR;
        }

        ht_table *functionTable= ht_getTableFor(&globalSymTable,actuallyParsedFunction);
        contextSymTable = functionTable;

        int err = 0;
        tokenTypes expresionType;
        err = expression(&expresionType);
        if (err != 0) {
            throwError(err, __LINE__);
            return err;
        }
        nextToken = getNextToken();
        if (nextToken->tokenType != END_OF_LINE) {
            throwError(SYNTAX_ERROR, __LINE__);
            return SYNTAX_ERROR;
        }
        err = fun_while_stat_list();
        if (err != 0) {
            throwError(err, __LINE__);
            return err;
        }
        nextToken = getNextToken();
        if (nextToken->tokenType != END_OF_LINE) {
            throwError(SYNTAX_ERROR, __LINE__);
            return SYNTAX_ERROR;
        }
        return 0;
    } else if (nextToken->tokenType == KEY_RETURN){ // 24. <fun-stat> -> KEY_RETURN <expression> END_OF_LINE

        ht_table *functionTable= ht_getTableFor(&globalSymTable,actuallyParsedFunction);
        contextSymTable = functionTable;

        int err = 0;
        tokenTypes expresionType;
        err = expression(&expresionType);
        if (err != 0) {
            throwError(err, __LINE__);
            return err;
        }

        nextToken = getNextToken();
        if (nextToken->tokenType != END_OF_LINE) {
            throwError(SYNTAX_ERROR, __LINE__);
            return SYNTAX_ERROR;
        }

        return 0;
    } else{
        throwError(SYNTAX_ERROR,__LINE__);
        return SYNTAX_ERROR;
    }
}

/*
 * <scope-while-stat-list>
 *  <scope-while-stat-list> -> <scope-stat> <scope-while-stat-list>
 *  <scope-while-stat-list> -> KEY_LOOP
 */
int scope_while_stat_list(){
    nextToken= getNextToken();
    if(nextToken->tokenType == KEY_LOOP){
        return 0;
    }

    int err = scope_stat();
    if(err != 0){
        throwError(err,__LINE__);
        return err;
    }

    return scope_while_stat_list();
}

/*
 * <fun-while-stat-list>
 *  <fun-while-stat-list> -> <fun-stat> <fun-while-stat-list>
 *  <fun-while-stat-list> -> KEY_LOOP
 */
int fun_while_stat_list(){
    nextToken= getNextToken();
    if(nextToken->tokenType == KEY_LOOP){
        return 0;
    }
    int err = fun_stat();
    if(err != 0){
        throwError(err,__LINE__);
        return err;
    }

    return fun_while_stat_list();
}

/*
 * <scope-else-stat-list>
 *  <scope-else-stat-list> -> <scope-stat> <scope-else-stat-list>
 *  <scope-else-stat-list> -> KEY_END KEY_IF
 */
int scope_else_stat_list(){
    nextToken= getNextToken();
    if(nextToken->tokenType == KEY_END){
        nextToken= getNextToken();
        if(nextToken->tokenType != KEY_IF){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
        }
        return 0;
    }

    int err = scope_stat();
    if(err != 0){
        throwError(err,__LINE__);
        return err;
    }

    return scope_else_stat_list();
}

/*
 * <fun-else-stat-list>
 *  <fun-if-stat-list> -> <fun-stat> <fun-else-stat-list>
 *  <fun-if-stat-list> -> KEY_END KEY_IF
 */
int fun_else_stat_list(){
    nextToken= getNextToken();
    if(nextToken->tokenType == KEY_END){
        nextToken= getNextToken();
        if (nextToken->tokenType != KEY_IF){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
        }
        return 0;
    }
    int err = fun_stat();
    if(err != 0){
        throwError(err,__LINE__);
        return err;
    }

    err = fun_else_stat_list();
    if(err != 0){
        throwError(err,__LINE__);
        return err;
    }
    return 0;
}

/*
 * <scope-if-stat-list>
 *  <scope-if-stat-list> -> <scope-stat> <scope-if-stat-list>
 *  <scope-if-stat-list> -> KEY_ELSE END_OF_LINE
 */
int scope_if_stat_list(){
    nextToken= getNextToken();
    if(nextToken->tokenType==KEY_ELSE){
        nextToken= getNextToken();
        if(nextToken->tokenType != END_OF_LINE){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
        }
        return 0;
    }

    int err = scope_stat();
    if(err != 0){
        throwError(err,__LINE__);
        return err;
    }

    return scope_if_stat_list();
}

/*
 * <fun-if-stat-list>
 *  <fun-if-stat-list> -> <fun-stat> <fun-if-stat-list>
 *  <fun-if-stat-list> -> KEY_ELSE END_OF_LINE
 */
int fun_if_stat_list(){
    nextToken= getNextToken();
    if(nextToken->tokenType==KEY_ELSE){
        nextToken= getNextToken();
        if(nextToken->tokenType != END_OF_LINE){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
        }
        return 0;
    }

    int err = fun_stat();
    if(err != 0){
        throwError(err,__LINE__);
        return err;
    }
    return fun_if_stat_list();
}

/*
 * <assign>
 *  <assign> -> OPERATOR_ASSIGN <expression> END_OF_LINE
 *  <assign> -> END_OF_LINE
 */
int assign(tokenTypes * expresionType){
    nextToken = getNextToken();
    if(nextToken->tokenType == END_OF_LINE){ // <assign> -> END_OF_LINE
        return 0;
    }
    if(nextToken->tokenType != OPERATOR_ASSIGN){    //<assign> -> OPERATOR_ASSIGN <expression> END_OF_LINE
        throwError(SYNTAX_ERROR,__LINE__);
        return SYNTAX_ERROR;
    }
    int err =0;
    err = expression(expresionType);
    if(err != 0){
        throwError(err,__LINE__);
        return err;
    }
    nextToken = getNextToken();
    if(nextToken->tokenType!= END_OF_LINE){
        throwError(SYNTAX_ERROR,__LINE__);
        return SYNTAX_ERROR;
    }
    return 0;
}

/*
 * <print-list>
 *  40. <print-list> -> <expression> SEMICOLON <print>
 */
int LLrule_print_list(){


    tokenTypes expresionType;
    int err = expression(&expresionType);
    if(err != 0){
        throwError(err,__LINE__);
        return err;
    }
    nextToken= getNextToken();
    if(nextToken->tokenType!= SEMICOLON){
        throwError(SYNTAX_ERROR,__LINE__);
        return SYNTAX_ERROR;
    }
    return LLrule_print();
}

/*
 * <print>
 *  <print> -> END_OF_LINE
 *  <print> -> <expression> SEMICOLON <print>
 */
int LLrule_print(){
    nextToken = getNextToken();
    if( nextToken->tokenType == END_OF_LINE){
        return 0;
    }

    /////////////////////////////////////////////////////// POZOR !! V NEXTTOKEN UZ JE TOKEN POTREBNY V EXPRESSION
    tokenTypes expresionType;
    int err = expression(&expresionType);                                         ///nejak si s tim porad, budouci ja
    if(err != 0){
        throwError(err,__LINE__);
        return err;
    }
    nextToken= getNextToken();
    if(nextToken->tokenType!= SEMICOLON){
        throwError(SYNTAX_ERROR,__LINE__);
        return SYNTAX_ERROR;
    }
    return LLrule_print();
}

/*
 * <param-list>
 * 9.  <param-list> -> CLOSING_BRACKET
 * 10. <param-list> -> IDENTIFIER KEY_AS <data-type> <param>
 */
int param_list(tFunctionParams * paramStack){
    nextToken = getNextToken();

    if(nextToken->tokenType == IDENTIFIER) {

        // new param;
        // param->name = nextToken->data;
        char *paramName;
        strcpy(paramName,nextToken->data);
        nextToken = getNextToken();
        if (nextToken->tokenType != KEY_AS) {
            throwError(SYNTAX_ERROR, __LINE__);
            return SYNTAX_ERROR;
        }

        int paramDataType = 0;
        int err = data_type(&paramDataType);
        if (err != 0) {
            throwError(err, __LINE__);
            return err;
        }
        //param->type= paramDataType;
        //paramStack.Push(param)
        if ( ! st_addParam(paramStack, paramName, paramDataType)) {
            throwError(INTERNAL_ERROR, __LINE__);
        }
        err = param(paramStack);
        if(err != 0) {
            throwError(err, __LINE__);
        }
        return err;
    }

    if( nextToken->tokenType == CLOSING_BRACKET){
        return 0;
    }

    return SYNTAX_ERROR;
}

/*
 * <param>
 * 11. <param> -> CLOSING_BRACKET
 * 11. <param> -> COMMA IDENTIFIER KEY_AS <data-type> <param>
 */
int param(tFunctionParams * paramStack){
    nextToken = getNextToken();
    if(nextToken->tokenType == CLOSING_BRACKET){    //11. <param> -> CLOSING_BRACKET
        return 0;
    }
    if(nextToken->tokenType == COMMA) {               //11. <param> -> COMMA IDENTIFIER KEY_AS <data-type> <param>
        nextToken = getNextToken();
        if (nextToken->tokenType != IDENTIFIER) {
            throwError(SYNTAX_ERROR, __LINE__);
            return SYNTAX_ERROR;
        }
        // new param;
        // param->name = nextToken->data;
        char *paramName;
        strcpy(paramName,nextToken->data);

        nextToken = getNextToken();
        if (nextToken->tokenType != KEY_AS) {
            throwError(SYNTAX_ERROR, __LINE__);
            return SYNTAX_ERROR;
        }
        int paramDataType = 0;
        int err = 0;
        err = data_type(&paramDataType);
        if (err != 0) {
            throwError(SYNTAX_ERROR, __LINE__);
            return SYNTAX_ERROR;
        }
        if ( ! st_addParam(paramStack, paramName, paramDataType)) {
            throwError(INTERNAL_ERROR, __LINE__);
        }
        //param->dataType = paramDataType;
        //paramStack.Push(param);

        return param(paramStack);
    }

    return SYNTAX_ERROR;
}

/*
 * <param-id-list>
 *  39. <param-id-list> -> CLOSING_BRACKET
 *  39. <param-id-list> -> <expression> <param-id>
 */
int param_id_list(){
    nextToken= getNextToken();
    if(nextToken->tokenType == CLOSING_BRACKET){
        return 0;
    }
    /*
    if(nextToken->tokenType == IDENTIFIER){
        return param_id();
    }
     */

    tokenTypes expresionType;
    int err = expression(&expresionType);///////////////////////////////////////////////////////////////////////Pozor!!! Nacteny Token
    if(err != 0) {
        throwError(err, __LINE__);
        return err;
    }
    return param_id();
}

/*
 * <param-id>
 *  11. <param> -> CLOSING_BRACKET
 *  11. <param> -> COMMA <expression> <param-id>
 */
int param_id(){
    nextToken= getNextToken();
    if(nextToken->tokenType == CLOSING_BRACKET){
        return 0;
    }
    if(nextToken->tokenType == COMMA){
        /*
        nextToken=getNextToken();
        if(nextToken->tokenType != IDENTIFIER){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
        }
         */

        tokenTypes expresionType;
        int err = expression(&expresionType);
        if(err != 0) {
            throwError(err, __LINE__);
            return err;
        }
        return param_id();
    }
    return SYNTAX_ERROR;
}

/**
 * Use as int a;
 * data_type(&a);
 *
 * @param pointer to int, will contain data type
 * @return return 0 if correct, SYNTAX_ERROR if not
 */
int data_type(int* type){
    nextToken = getNextToken();
    if(nextToken->tokenType == KEY_STRING){
        *type = STRING;
        return 0;
    }
    if(nextToken->tokenType == KEY_DOUBLE){
       *type = DOUBLE;
        return 0;
    }
    if(nextToken->tokenType == KEY_INTEGER){
       *type = INTEGER;
        return 0;
    }
   //throwError(SYNTAX_ERROR,__LINE__);
    return SYNTAX_ERROR;
}
