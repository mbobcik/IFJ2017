//
// Created by Martin Bobčík on 11/23/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "error.h"
#include "scanner.h"
#include "parser.h"
#include "functions.h"
#include "symtable.h"

token * nextToken;

int parse(){

    // init symbol table

    // add  built-in functions

    int err = prog();
    if(err != 0){
        return err;
    }

    return 0;
}
 /*
  * 1.  <prog> -> KEY_SCOPE END_OF_LINE <scope-st-list>
  * 2.  <prog> -> KEY_DECLARE KEY_FUNCTION IDENTIFIER OPENING_BRACKET <param-list> KEY_AS <data-type> END_OF_LINE <prog>
  * 4.  <prog> -> KEY_FUNCTION IDENTIFIER OPENING_BRACKET <param-list> KEY_AS <data-type> END_OF_LINE <fun-st-list>
  * 5.  <prog> -> END_OF_LINE <prog>
  */
int prog(){
     int err;
     nextToken = getToken();

     if( nextToken->tokenType == KEY_SCOPE){        // 1. <prog> -> KEY_SCOPE END_OF_LINE <scope-st-list>
         nextToken = getToken();
         if (nextToken->tokenType != END_OF_LINE){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
         }

         return scope_st_list();

     } else if (nextToken->tokenType == KEY_DECLARE) {   //2. <prog> -> KEY_DECLARE KEY_FUNCTION IDENTIFIER OPENING_BRACKET <param-list> KEY_AS <data-type> END_OF_LINE <prog>

         nextToken = getToken();
         if (nextToken->tokenType != KEY_FUNCTION) {
            throwError(SYNTAX_ERROR, __LINE__);
            return SYNTAX_ERROR;
         }

         nextToken = getToken();
         if (nextToken->tokenType != IDENTIFIER){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
         }
         // check redeclaration
         // symtable->functionName = nextToken->data

         nextToken = getToken();
         if (nextToken->tokenType != OPENING_BRACKET){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
         }

         err = param_list();
         if(err != 0){
             throwError(err,__LINE__);
             return err;
         }

         // somehow stuff parameters to symtable;

         nextToken = getToken();
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
         // symtable->functionDataType = actualDataType;

         nextToken = getToken();
         if (nextToken->tokenType != END_OF_LINE){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
         }

         err = prog();
         throwError(err,__LINE__);
         return err;
     } else if (nextToken->tokenType == KEY_FUNCTION){ // 4.  <prog> -> KEY_FUNCTION IDENTIFIER OPENING_BRACKET <param-list>  KEY_AS <data-type> END_OF_LINE <fun-st-list>

         nextToken = getToken();
         if(nextToken->tokenType != IDENTIFIER){
             throwError(SYNTAX_ERROR,__LINE__);
             return SYNTAX_ERROR;
         }

         //maybe chech if declared

         //check redefinition

         //symtable->funcionName = nextToken->data;

         nextToken=getToken();
         if(nextToken->tokenType != OPENING_BRACKET){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
         }

         err = param_list();
         if(err != 0){
             throwError(err,__LINE__);
             return err;
         }
         //stuff params to symtable

         nextToken = getToken();
         if(nextToken->tokenType != KEY_AS){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
         }

         int actualDataType=0;
         err = data_type(&actualDataType);
         //symtable->functionType = actualDataType ,,,, or check Datatype

         nextToken = getToken();
         if (nextToken->tokenType != END_OF_LINE){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
         }

         return fun_st_list();
     } else if (nextToken->tokenType == END_OF_LINE){
        prog();
     } else{
        throwError(SYNTAX_ERROR,__LINE__);
        return SYNTAX_ERROR;
     }

     return 0;
}

int fun_st_list(){
    return 0;
}

/*
 * <end-prog>
 *  7.  <end-prog> -> END_OF_LINE <end-prog>
 *  8.  <end-prog> -> END_OF_FILE
 */
int end_prog(){
    nextToken = getToken();
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
 * 25. <scope-st-list> -> <scope-stat> <scope-st-list>
 * 28. <scope-st-list> -> KEY_END KEY_SCOPE <end-prog>
 */
int scope_st_list(){
   nextToken= getToken();
    if(nextToken->tokenType == KEY_END){    //28. <scope-st-list> -> KEY_END KEY_SCOPE <end-prog>
        nextToken=getToken();
        if(nextToken->tokenType != KEY_SCOPE){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
        }
        return end_prog();
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
 *  29. <scope-stat> -> KEY_DIM IDENTIFIER KEY_AS <data-type> <assign>
 *  31. <scope-stat> -> IDENTIFIER OPERATOR_ASSIGN <expression> END_OF_LINE
 *  32. <scope-stat> -> KEY_INPUT IDENTIFIER END_OF_LINE
 *  33. <scope-stat> -> KEY_PRINT <print-list> END_OF_LINE
 *  34. <scope-stat> -> KEY_IF <expression> KEY_THEN END_OF_LINE <scope-if-stat-list> <scope-else-stat-list> END_OF_LINE
 *  35. <scope-stat> -> KEY_DO KEY_WHILE <expression> END_OF_LINE <scope-while-stat-list> END_OF_LINE
*/
int scope_stat(){
    if(nextToken->tokenType == END_OF_LINE){ // 29. <scope-stat> -> END_OF_LINE
        return 0;
    } else if (nextToken->tokenType == KEY_DIM){ // 29. <scope-stat> -> KEY_DIM IDENTIFIER KEY_AS <data-type> <assign>
        nextToken = getToken();
        if(nextToken->tokenType != IDENTIFIER){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
        }
        //symTable newVariable.VarName = nextToken->data

        nextToken=getToken();
        if(nextToken->tokenType!= KEY_AS){
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
        // maybe VarType = nextToken->tokenType could be easier dum**ss

        return assign();

    } else if (nextToken->tokenType == IDENTIFIER){ //  31. <scope-stat> -> IDENTIFIER OPERATOR_ASSIGN <expression> END_OF_LINE
        nextToken = getToken();
        if(nextToken->tokenType != OPERATOR_ASSIGN){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
        }

        int err =0;
        err = expression();
        if(err != 0){
            throwError(err,__LINE__);
            return err;
        }

        nextToken=getToken();
        if (nextToken->tokenType!=END_OF_LINE){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
        }
        return 0;
    } else if (nextToken->tokenType == KEY_INPUT){ //  32. <scope-stat> -> KEY_INPUT IDENTIFIER END_OF_LINE
        nextToken = getToken();
        if(nextToken->tokenType!= IDENTIFIER){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
        }

        //symtable.checkVarName();
        nextToken=getToken();
        if (nextToken->tokenType != END_OF_LINE){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
        }
        return 0;
    } else if (nextToken->tokenType == KEY_PRINT){ // 33. <scope-stat> -> KEY_PRINT <print-list> END_OF_LINE

        int err =0;
        err = print_list();
        if(err != 0){
            throwError(err,__LINE__);
            return err;
        }
        nextToken=getToken();
        if (nextToken->tokenType != END_OF_LINE){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
        }
        return 0;
    } else if (nextToken->tokenType == KEY_IF){ //  <scope-stat> -> KEY_IF <expression> KEY_THEN END_OF_LINE <scope-if-stat-list> <scope-else-stat-list> END_OF_LINE
        int err =0;
        err = expression();
        if(err != 0){
            throwError(err,__LINE__);
            return err;
        }

        nextToken = getToken();
        if(nextToken->tokenType!= KEY_THEN){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
        }

        nextToken = getToken();
        if(nextToken->tokenType!= END_OF_LINE){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
        }

        err = scope_if_stat_list();
        if(err != 0){
            throwError(err,__LINE__);
            return err;
        }

        nextToken = getToken();
        if(nextToken->tokenType!= KEY_ELSE){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
        }

        err = scope_else_stat_list();
        if(err != 0){
            throwError(err,__LINE__);
            return err;
        }
        nextToken = getToken();
        if(nextToken->tokenType!= END_OF_LINE){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
        }

        return 0;
    } else if (nextToken->tokenType == KEY_DO){ // 35. <scope-stat> -> KEY_DO KEY_WHILE <expression> END_OF_LINE <scope-while-stat-list> END_OF_LINE
        nextToken = getToken();
        if(nextToken->tokenType!= KEY_WHILE){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
        }
        int err =0;
        err = expression();
        if(err != 0){
            throwError(err,__LINE__);
            return err;
        }
        nextToken = getToken();
        if(nextToken->tokenType!= END_OF_LINE){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
        }
        err = scope_while_stat_list();
        if(err != 0){
            throwError(err,__LINE__);
            return err;
        }
        nextToken = getToken();
        if(nextToken->tokenType!= END_OF_LINE){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
        }
        return 0;
    }
    throwError(SYNTAX_ERROR,__LINE__);
    return SYNTAX_ERROR;
}

int fun_stat(){
    return 0;
}

int scope_while_stat_list(){
    return 0;
}

int fun_while_stat_list(){
    return 0;
}

int scope_else_stat_list(){
    return 0;
}

/*
 * <fun-else-stat-list>
 *  <fun-if-stat-list> -> <fun-stat> <fun-else-stat-list>
 *  <fun-if-stat-list> -> KEY_END KEY_IF
 */
int fun_else_stat_list(){
    nextToken=getToken();
    if(nextToken->tokenType == KEY_END){
        nextToken=getToken();
        if (nextToken->tokenType != KEY_IF){
            throwError(SYNTAX_ERROR,__LINE__);
            return SYNTAX_ERROR;
        }
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

int scope_if_stat_list(){
    return 0;
}

/*
 * <fun-if-stat-list>
 *  <fun-if-stat-list> -> <fun-stat> <fun-if-stat-list>
 *  <fun-if-stat-list> -> KEY_ELSE END_OF_LINE
 */
int fun_if_stat_list(){
    return 0;
}

/*
 * <assign>
 *  <assign> -> OPERATOR_ASSIGN <expression> END_OF_LINE
 *  <assign> -> END_OF_LINE
 */
int assign(){
    nextToken=getToken();
    if(nextToken->tokenType == END_OF_LINE){ // <assign> -> END_OF_LINE
        return 0;
    }
    if(nextToken->tokenType != OPERATOR_ASSIGN){    //<assign> -> OPERATOR_ASSIGN <expression> END_OF_LINE
        throwError(SYNTAX_ERROR,__LINE__);
        return SYNTAX_ERROR;
    }
    int err =0;
    err = expression();
    if(err != 0){
        throwError(err,__LINE__);
        return err;
    }
    nextToken = getToken();
    if(nextToken->tokenType!= END_OF_LINE){
        throwError(SYNTAX_ERROR,__LINE__);
        return SYNTAX_ERROR;
    }
    return 0;
}

/*
 * <print-list>
 *  40. <print-list> -> <expression> SEMICOLON <print-list>
 *  41. <print-list> -> <expression> SEMICOLON
 */
int print_list(){
    return 0;
}

/*
 * <param-list>
 * 9.  <param-list> -> CLOSING_BRACKET
 * 10. <param-list> -> IDENTIFIER KEY_AS <data-type> <param>
 */
int param_list(){
    nextToken = getToken();

    if( nextToken->tokenType == CLOSING_BRACKET){
        return 0;
    }

    if(nextToken->tokenType != IDENTIFIER){
        throwError(SYNTAX_ERROR,__LINE__);
        return SYNTAX_ERROR;
    }

    // new param;
    // param->name = nextToken->data;

    nextToken=getToken();
    if (nextToken->tokenType != KEY_AS){
       throwError(SYNTAX_ERROR,__LINE__);
         return SYNTAX_ERROR;
    }

    int paramDataType = 0;
    int err = data_type(&paramDataType);
    if(err != 0){
        throwError(err,__LINE__);
        return err;
    }
    //param->type= paramDataType;
    //paramStack.Push(param)

    err = param();
    throwError(err,__LINE__);
    return err;
}

int param_id_list(){
    return 0;
}

/*
 * <param>
 * 11. <param> -> CLOSING_BRACKET
 * 11. <param> -> COMMA IDENTIFIER KEY_AS <data-type> <param>
 */
int param(){
    nextToken = getToken();
    if(nextToken->tokenType == CLOSING_BRACKET){    //11. <param> -> CLOSING_BRACKET
        return 0;
    }
    if(nextToken->tokenType != COMMA){               //11. <param> -> COMMA IDENTIFIER KEY_AS <data-type> <param>
       throwError(SYNTAX_ERROR,__LINE__);
         return SYNTAX_ERROR;
    }
    nextToken=getToken();
    if(nextToken->tokenType != IDENTIFIER){
       throwError(SYNTAX_ERROR,__LINE__);
        return SYNTAX_ERROR;
    }
    // new param;
    // param->name = nextToken->data;

    nextToken=getToken();
    if(nextToken->tokenType!=KEY_AS){
       throwError(SYNTAX_ERROR,__LINE__);
         return SYNTAX_ERROR;
    }
    int paramDataType=0;
    int err=0;
    err = data_type(&paramDataType);
    if(err != 0){
       throwError(SYNTAX_ERROR,__LINE__);
        return SYNTAX_ERROR;
    }
    //param->dataType = paramDataType;
    //paramStack.Push(param);

    return param();
}

int param_id(){
    return 0;
}

/**
 * Use as int a;
 * data_type(&a);
 *
 * @param pointer to int, will contain data type
 * @return return 0 if correct, SYNTAX_ERROR if not
 */
int data_type(int* type){
    nextToken = getToken();
    if(nextToken->tokenType == KEY_STRING){
        *type = KEY_STRING;
        return 0;
    }
    if(nextToken->tokenType == KEY_DOUBLE){
       *type = KEY_DOUBLE;
        return 0;
    }
    if(nextToken->tokenType == KEY_INTEGER){
       *type = KEY_INTEGER;
        return 0;
    }
   throwError(SYNTAX_ERROR,__LINE__);
    return SYNTAX_ERROR;
}

int expression(){
    return 0;
}
