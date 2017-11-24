//
// Created by Martin Bobčík on 11/23/17.
//

#ifndef SRC_PARSER_H
#define SRC_PARSER_H

int assign();
int data_type(int* type);
int end_prog();
int expression();
int fun_else_stat_list();
int fun_if_stat_list();
int fun_st_list();
int fun_stat();
int fun_while_stat_list();
int param();
int param_id();
int param_id_list();
int param_list();
int parse();
int print_list();
int prog();
int scope_else_stat_list();
int scope_if_stat_list();
int scope_st_list();
int scope_stat();
int scope_while_stat_list();


#endif //SRC_PARSER_H
