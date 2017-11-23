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
  * 4.  <prog> -> KEY_FUNCTION IDENTIFIER OPENING_BRACKET <param-list> CLOSING_BRACKET KEY_AS <data-type> END_OF_LINE <fun-st-list> KEY_END KEY_FUNCTION END_OF_LINE <prog>
  * 5.  <prog> -> END_OF_LINE <prog>
  * 6.  <prog> -> <end-prog>
  */
int prog(){
     int err;
     nextToken = getToken();

     if( nextToken->tokenType == KEY_SCOPE){        // 1. <prog> -> KEY_SCOPE END_OF_LINE <scope-st-list>
         nextToken = getToken();
         if (nextToken->tokenType != END_OF_LINE){
             return SYNTAX_ERROR;
         }

         err = scope_st_list();
         if(err != 0){
             return err;
         }

     } else if (nextToken->tokenType == KEY_DECLARE){   //2. <prog> -> KEY_DECLARE KEY_FUNCTION IDENTIFIER OPENING_BRACKET <param-list> KEY_AS <data-type> END_OF_LINE <prog>

         nextToken=getToken();
         if (nextToken->tokenType != KEY_FUNCTION)
             return SYNTAX_ERROR;

         nextToken = getToken();
         if (nextToken->tokenType != IDENTIFIER){
             return SYNTAX_ERROR;
         }
         // symtable->functionName = nextToken->data

         nextToken = getToken();
         if (nextToken->tokenType != OPENING_BRACKET){
             return SYNTAX_ERROR;
         }

         err = param_list();
         if(err != 0){
             return err;
         }
         // somehow stuff parameters to symtable;

         nextToken = getToken();
         if(nextToken->tokenType != KEY_AS){
             return SYNTAX_ERROR;
         }

         int actualDataType = 0;
         err = data_type(&actualDataType); // in Dtype will be actual type
         if(err != 0){
             return err;
         }
         // symtable->functionDataType = actualDataType;

         nextToken = getToken();
         if (nextToken->tokenType != END_OF_LINE){
             return SYNTAX_ERROR;
         }

         err = prog();
         return err;
     } else if (nextToken->tokenType == KEY_FUNCTION){

     } else if (nextToken->tokenType == END_OF_LINE){

     } else{
         return SYNTAX_ERROR;
     }

     return 0;
}

int end_prog(){
    return 0;
}

int scope_st_list(){
    return 0;
}

int param_list(){
    return 0;
}

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
    return SYNTAX_ERROR;
}