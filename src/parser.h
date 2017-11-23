//
// Created by Martin Bobčík on 11/23/17.
//

#ifndef SRC_PARSER_H
#define SRC_PARSER_H

int parse();
int prog();
int end_prog();
int scope_st_list();
int fun_st_list();
int scope_stat();
int param_list();
int data_type(int* type);
int param();
int assign();
int expression();
int print_list();
int scope_if_stat_list();
int scope_else_stat_list();
int scope_while_stat_list();
#endif //SRC_PARSER_H
