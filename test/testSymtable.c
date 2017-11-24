//
// Created by root on 11/23/17.
//

#include "../src/functions.h"

#include "../src/error.h"


int main() {

    printf("\ninit\n");

    ht_table  table;
    ht_init(&table);
    if (&table == NULL) {
        printf("\nchyba init\n");
    }
    show_ht_table(&table, false);


    printf("\nadd variable\n");
    ht_addVariable(&table, "test", INTEGER);
    show_ht_table(&table, false);

    printf("\nset value\n");
    int * a = malloc(sizeof(int));
    *a = 12;
    ht_setVarValue(&table, "test", a);
    show_ht_table(&table, false);



    printf("\nadd varible and set String\n");
    ht_addVariable(&table, "testString", STRING);
    char * text = malloc(sizeof(char)*5);
    text[0] = 't';
    text[1] = 'e';
    text[2] = 's';
    text[3] = 't';
    text[4] = '\0';

    ht_setVarValue(&table, "testString", text);
    show_ht_table(&table, false);



    printf("\nadd variable double\n");
    ht_addVariable(&table, "testDouble", DOUBLE);
    show_ht_table(&table, false);

    printf("\nset value\n");
    double * d = malloc(sizeof(double));
    *d = 12.23;
    ht_setVarValue(&table, "testDouble", d);
    show_ht_table(&table, false);


    printf("\nclear\n");
    ht_clearAll(&table);


    printf("\nadd existujici\n");
    tItem * retDoubleInsert = ht_addVariable(&table, "testExist", DOUBLE);
    if (retDoubleInsert != NULL) {
        printf("1. add ok\n");
    }

    retDoubleInsert = ht_addVariable(&table, "testExist", DOUBLE);
    if (retDoubleInsert != NULL) {
        printf("2. add bad\n");
    }
    else {
        printf("2. add ok\n");
    }

    show_ht_table(&table, false);


    printf("\nZiskani hodnoty: \n");
    printf("\n\tneexistujici: ");
    void * val = ht_getVarValue(&table, "testNotExist");
    if (val == NULL) {
        printf("OK\n");
    }
    else{
        printf("BAD\n");
    }

    printf("\n\texistujici NULL: ");
    val = ht_getVarValue(&table, "testExist");
    if (val == NULL) {
        printf("OK\n");
    }
    else{
        printf("BAD\n");
    }
    printf("\n\tdefinovana: ");
    if (ht_isVarDefined(&table, "testExist")) {
        printf("BAD - je definovana\n");
    }
    else {
        printf("OK - neni definovana\n");
    }


    double * d2 = malloc(sizeof(double));
    *d2 = 12.23;
    ht_setVarValue(&table, "testExist", d2);

    printf("\n\texistujici val: ");
    val = ht_getVarValue(&table, "testExist");
    if (val == NULL) {
        printf("BAD\n");
    }
    else{
        printf("OK\n");
    }


    printf("\n test exist:\n");
    printf("\n\tnot exist: ");
    if (ht_isVarExist(&table, "testExist2")) {
        printf("bad\n");
    }
    else {
        printf("ok\n");
    }
    retDoubleInsert = ht_addVariable(&table, "testExist2", DOUBLE);
    printf("\n\texist: ");
    if (ht_isVarExist(&table, "testExist2")) {
        printf("ok\n");
    }
    else {
        printf("bad\n");
    }

    retDoubleInsert = ht_addFunction(&table, "testExist2fce", DOUBLE);
    printf("\n\texist fce: ");
    if (ht_isVarExist(&table, "testExist2fce") == 1) {
        printf("bad\n");
    }
    else {
        printf("ok\n");
    }



    ht_clearAll(&table);

    printf("\nadd function: ");
    retDoubleInsert = ht_addFunction(&table, "testExist3", DOUBLE);
    if (retDoubleInsert != NULL) {
        printf("ok\n");
    }
    else {
        printf("bad\n");
    }

    show_ht_table(&table, false);



    // set value to function
    printf("\n set value to function: ");
    ht_addFunction(&table, "testFunkce", DOUBLE);
    double * d3 = malloc(sizeof(double));
    *d3 = 12.23;
    switch (ht_setVarValue(&table, "testFunkce", d3)) {
        case 1:
            printf("bad - proslo\n");
            break;
        case 0:
            printf("bad - not table\n");
            break;
        case -1:
            printf("bad - not exist\n");
            break;
        case -2:
            printf("ok - je to fce\n");
            break;
        case -3:
            printf("bad - interni\n");
            break;
        default:
            printf("bad - not spec\n");
            break;
    }
    free(d3);


    // get tabulku symbolu pro fci
    printf("\n add to function table: ");
    ht_table * fceTable = ht_getTableFor(&table, "testFunkce");
    retDoubleInsert = ht_addVariable(fceTable, "testVar1", DOUBLE);
    if (retDoubleInsert == NULL) {
        printf("bad\n");
    } else {
        printf("ok\n");
    }

    show_ht_table(&table, false);


    // nastaveni funkce ze je definovana
    printf("\n set func defined: ");
    ht_setFuncDefined(&table, "testFunkce");
    // kontrola
    if (ht_isFuncDefined(&table, "testFunkce")) {
        printf("ok\n");
    }
    else {
        printf("bad\n");
    }

    show_ht_table(&table, false);




    //je vse definovano?
    printf("\n is all defined: ");
    if ( ! ht_isAllDefined(&table)) {
        printf("ok - not defined\n");
    } else {
        printf("bad - all defined\n");
    }


    // dodefinovani

    printf("\nDodefinovani...\n");
    ht_setFuncDefined(&table, "testExist3");
    fceTable = ht_getTableFor(&table, "testFunkce");

    double * d4 = malloc(sizeof(double));
    *d4 = 12.23;
    ht_setVarValue(fceTable, "testVar1", d4);

    printf("\n is all defined: ");
    if (ht_isAllDefined(&table)) {
        printf("ok - all defined\n");
    } else {
        printf("bad - not defined\n");
    }


    show_ht_table(&table, false);








    // parametry
    printf("\n\nParametry: \n");

    tFunctionParams * list = NULL;
    st_init(list);

    show_st_list(list);

    printf("\n create param: ");
    tItem * param = ht_creatItem("param1", DOUBLE);
    if (param == NULL) {
        printf("bad\n");
    } else {
        printf("ok\n");
    }

    printf("\n add param: ");
    if (st_add(list,param)) {
        printf("ok\n");
    }
    else {
        printf("bad\n");
    }

    show_st_list(list);


    printf("\n clear\n");
    ht_clearAll(&table);
    show_ht_table(&table, false);


    return 0;
}