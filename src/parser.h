//
// Created by Martin Bobčík on 11/23/17.
//

#ifndef SRC_PARSER_H
#define SRC_PARSER_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "error.h"
#include "scanner.h"
#include "functions.h"
#include "symtable.h"


int assign();
int data_type(int* type);
int end_prog();
int fun_else_stat_list();
int fun_if_stat_list();
int fun_st_list();
int fun_stat();
int fun_while_stat_list();
int param(tFunctionParams * paramStack);
int param_id();
int param_id_list();
int param_list(tFunctionParams * paramStack);
int parse();
int LLrule_print_list();
int LLrule_print();
int prog();
int scope_else_stat_list();
int scope_if_stat_list();
int scope_st_list();
int scope_stat();
int scope_while_stat_list();

token * nextToken;
ht_table globalSymTable;
char *actuallyParsedFunction;

#endif //SRC_PARSER_H
