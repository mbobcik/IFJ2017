//
// Created by root on 11/23/17.
//

#include "../src/functions.h"

#include "../src/error.h"


int main() {

    // Vytvoreni tabulky
    ht_table  tabulkaSymbolu;
    ht_init(&tabulkaSymbolu);
    if (&tabulkaSymbolu == NULL) {
        printf("\nChyba init\n");
    }


    /// Funkce

    // Pridani funkce (bez parametru)
    if (ht_addFunction(&tabulkaSymbolu, "funkce1", DOUBLE) == NULL) {
        printf("\nChyba pri zakladani funkce.\n");
    }

    // Prametry
    tFunctionParams  * params;
    st_init(&params);

    // add param
    if ( ! st_addParam(params, "param1", DOUBLE)) {
        printf("\nChyba pro zakladani parametru.\n");
    }

    // Pridani funkce (s parametry)
    if (ht_addFunctionWithParams(&tabulkaSymbolu, "funkce2", DOUBLE, params) == NULL) {
        printf("\nChyba pri zakladani funkce.\n");
    }

    // Prametry pozdejsi doplneni do funkce
    tFunctionParams  * params2;
    st_init(&params2);

    // add param
    if ( ! st_addParam(params2, "param1", DOUBLE)) {
        printf("\nChyba pro zakladani parametru.\n");
    }

    if ( ! ht_setFuncParams(&tabulkaSymbolu, "funkce1", params2)) {
        printf("\nChyba pri nastavovani parametru\n");
    }


    // Je to funkce?

    // Neznam parametry
    if (ht_isFuncExist(&tabulkaSymbolu, "funkce1") != HT_FUNC_OK) {
        printf("\nFunkce neexistuje.\n");
    }

    // Znam parametry a chci si overit jestli je takhle deklarovana
    // Prametry pozdejsi doplneni do funkce
    tFunctionParams  * params3;
    st_init(&params3);

    // add param
    if ( ! st_addParam(params3, "param1", DOUBLE)) {
        printf("\nChyba pro zakladani parametru.\n");
    }
    if (! (ht_isFunction(&tabulkaSymbolu, "funkce1", DOUBLE, params3) == HT_FUNC_OK)) {
        printf("\nTakova funkce deklarovana neni.\n");
    }


    // Nastaveni priznaku definovana pro funkci
    ht_setFuncDefined(&tabulkaSymbolu, "funkce1");

    // get tabulku symbolu
    ht_getTableFor(&tabulkaSymbolu, "funkce1");


    /// Promenne

    // add
    if (ht_addVariable(&tabulkaSymbolu, "promennaA", DOUBLE) == NULL) {
        printf("\nNepodarilo se pridat promennou.\n");
    }

    // set value (hodnota je malloc a bude zkopirovane pomoci memcpy)
    if (ht_setVarValueDouble(&tabulkaSymbolu, "promennaA", 10.0) != HT_VAR_OK) {
        printf("\nNepodarilo se setnout promennou.\n");
    }

    // exists
    if (ht_isVarExist(&tabulkaSymbolu, "promennaA") != HT_VAR_OK) {
        printf("\nNeexistujici.\n");
    }

    // nastaveni hodnoty promenne ve funkci
    //ht_setVarValueDoubleForFunc()





    show_ht_table(&tabulkaSymbolu, false);


    /*

    bool allTestsPass = true;

    ht_table  table;
    ht_init(&table);
    if (&table == NULL) {
        printf("\nchyba init\n");
    }
    //show_ht_table(&table, false);


    ht_addVariable(&table, "test", INTEGER);
    //show_ht_table(&table, false);

    int * a = malloc(sizeof(int));
    *a = 12;
    ht_setVarValue(&table, "test", a);
    //show_ht_table(&table, false);



    ht_addVariable(&table, "testString", STRING);
    char * text = malloc(sizeof(char)*5);
    text[0] = 't';
    text[1] = 'e';
    text[2] = 's';
    text[3] = 't';
    text[4] = '\0';

    ht_setVarValue(&table, "testString", text);
    //show_ht_table(&table, false);

    ht_addVariable(&table, "testDouble", DOUBLE);
    //show_ht_table(&table, false);

    double * d = malloc(sizeof(double));
    *d = 12.23;
    ht_setVarValue(&table, "testDouble", d);
    //show_ht_table(&table, false);

    ht_clearAll(&table);


    printf("\nAdd existujici\n");
    tItem * retDoubleInsert = ht_addVariable(&table, "testExist", DOUBLE);
    if (retDoubleInsert != NULL) {
        printf("  1. add ok\n");
    } else {
        allTestsPass = false;
    }

    retDoubleInsert = ht_addVariable(&table, "testExist", DOUBLE);
    if (retDoubleInsert != NULL) {
        printf("  2. add bad\n");
        allTestsPass = false;
    }
    else {
        printf("  2. add ok\n");
    }

    //show_ht_table(&table, false);


    printf("\nZiskani hodnoty: \n");
    printf("\n  neexistujici: ");
    void * val = ht_getVarValue(&table, "testNotExist");
    if (val == NULL) {
        printf("OK\n");
    }
    else{
        printf("BAD\n");
        allTestsPass = false;
    }

    printf("\n  existujici NULL: ");
    val = ht_getVarValue(&table, "testExist");
    if (val == NULL) {
        printf("OK\n");
    }
    else{
        printf("BAD\n");
        allTestsPass = false;
    }

    printf("\n  definovana: ");
    if (ht_isVarDefined(&table, "testExist")) {
        printf("BAD - je definovana\n");
        allTestsPass = false;
    }
    else {
        printf("OK - neni definovana\n");
    }


    double * d2 = malloc(sizeof(double));
    *d2 = 12.23;
    ht_setVarValue(&table, "testExist", d2);

    printf("\n  existujici val: ");
    val = ht_getVarValue(&table, "testExist");
    if (val == NULL) {
        printf("BAD\n");
        allTestsPass = false;
    }
    else{
        printf("OK\n");
    }


    printf("\nExistence:\n");
    printf("\n  not exist: ");
    if (ht_isVarExist(&table, "testExist2") == HT_VAR_OK) {
        printf("bad\n");
        allTestsPass = false;
    }
    else {
        printf("ok\n");
    }

    retDoubleInsert = ht_addVariable(&table, "testExist2", DOUBLE);
    printf("\n  exist: ");
    if (ht_isVarExist(&table, "testExist2") == HT_VAR_OK) {
        printf("ok\n");
    }
    else {
        printf("bad\n");
        allTestsPass = false;
    }

    retDoubleInsert = ht_addFunction(&table, "testExist2fce", DOUBLE);
    printf("\n  exist fce: ");
    if (ht_isVarExist(&table, "testExist2fce") == HT_VAR_OK) {
        printf("bad\n");
        allTestsPass = false;
    }
    else {
        printf("ok\n");
    }



    ht_clearAll(&table);

    printf("\nFunkce: \n");
    printf("\n  Add function: ");
    retDoubleInsert = ht_addFunction(&table, "testExist3", DOUBLE);
    if (retDoubleInsert != NULL) {
        printf("ok\n");
    }
    else {
        printf("bad\n");
        allTestsPass = false;
    }

    //show_ht_table(&table, false);



    // set value to function
    printf("\n  Set vall to func: ");
    ht_addFunction(&table, "testFunkce", DOUBLE);
    double * d3 = malloc(sizeof(double));
    *d3 = 12.23;
    switch (ht_setVarValue(&table, "testFunkce", d3)) {
        case HT_VAR_OK:
            printf("bad - proslo\n");
            allTestsPass = false;
            break;
        case HT_NULL_TABLE:
            printf("bad - not table\n");
            allTestsPass = false;
            break;
        case HT_VAR_NOT_FOUD:
            printf("bad - not exist\n");
            allTestsPass = false;
            break;
        case HT_VAR_NOT_VAR:
            printf("ok - je to fce\n");
            break;
        case HT_ITERNAL_ERROR:
            printf("bad - interni\n");
            allTestsPass = false;
            break;
        default:
            printf("bad - not spec\n");
            allTestsPass = false;
            break;
    }
    free(d3);


    // get tabulku symbolu pro fci
    printf("\n  add to func table: ");
    ht_table * fceTable = ht_getTableFor(&table, "testFunkce");
    retDoubleInsert = ht_addVariable(fceTable, "testVar1", DOUBLE);
    if (retDoubleInsert == NULL) {
        printf("bad\n");
        allTestsPass = false;
    } else {
        printf("ok\n");
    }

    //show_ht_table(&table, false);


    // nastaveni funkce ze je definovana
    printf("\n  set func defined: ");
    ht_setFuncDefined(&table, "testFunkce");
    // kontrola
    if (ht_isFuncDefined(&table, "testFunkce")) {
        printf("ok\n");
    }
    else {
        printf("bad\n");
        allTestsPass = false;
    }

    //show_ht_table(&table, false);



    printf("\nDefinovani: \n");

    //je vse definovano?
    printf("\n  Is all defined: ");
    if ( ! ht_isAllDefined(&table)) {
        printf("ok - not defined\n");
    } else {
        printf("bad - all defined\n");
        allTestsPass = false;
    }


    // dodefinovani

    printf("\n  Dodefinovani...\n");
    ht_setFuncDefined(&table, "testExist3");
    fceTable = ht_getTableFor(&table, "testFunkce");

    double * d4 = malloc(sizeof(double));
    *d4 = 12.23;
    ht_setVarValue(fceTable, "testVar1", d4);

    printf("\n  is all defined: ");
    if (ht_isAllDefined(&table)) {
        printf("ok - all defined\n");
    } else {
        printf("bad - not defined\n");
        allTestsPass = false;
    }


    //show_ht_table(&table, false);


    // parametry
    printf("\n\nParametry: \n");

    tFunctionParams  * list;
    st_init(&list); // init params

    show_st_list(list);

    printf("\n  create param: ");
    tItem * param = ht_creatItem("param1", DOUBLE);
    if (param == NULL) {
        printf("bad\n");
        allTestsPass = false;
    } else {
        printf("ok\n");
        param->type = parametr;
    }

    printf("\n  add param: ");
    if (st_add(list, param)) {
        printf("ok");
    }
    else {
        printf("bad");
        allTestsPass = false;
    }

    show_st_list(list);
    printf("\n");

    printf("\nclear list: ");
    st_clear(list);
    show_st_list(list);



    // add params to function
    tFunctionParams  * listParams;
    st_init(&listParams); // init params

    // add param
    printf("\n  add param: ");
    if ( ! st_addParam(listParams, "param1", DOUBLE)) {
        printf("BAD - cant add param\n");
        allTestsPass = false;
    }
    else {
        printf("Ok - added\n");
    }

    // add same sane
    printf("\n  try add same name param: ");
    if (st_addParam(listParams, "param1", DOUBLE)) {
        printf("BAD - can add same param\n");
        allTestsPass = false;
    }
    else {
        printf("OK - not add same name param\n");
    }

    printf("\n  add next param: ");
    if (st_addParam(listParams, "param2", INTEGER)) {
        printf("ok - add next param\n");
    }
    else {
        printf("bad - cant add next\n");
        allTestsPass = false;
    }

    // vytvoreni funkce rovnou s parametry
    printf("\n  create func w/ params: ");
    tItem * func = ht_addFunctionWithParams(&table, "funcCreateWithParams", INTEGER, listParams);
    if (func == NULL) { // chck create
        printf("bad - cant create\n");
        allTestsPass = false;
    } else {
        tFunctionData * data = func->data;
        if (data == NULL) { // check data
            printf("bad - no data\n");
            allTestsPass = false;
        } else {
            if (data->params == NULL) { // chck params list
                printf("bad - no params\n");
                allTestsPass = false;
            }
            else {
                // chcek params val
                if (data->params->item == NULL) { // check first
                    printf("bad - first not init\n");
                    allTestsPass = false;
                } else {
                    if (strcmp(data->params->item->name, "param1") != 0) {
                        printf("bad - not first param\n");
                        allTestsPass = false;
                    } else {
                        printf("ok - add & set params\n");
                    }
                }
            }
        }
    }

    // vytvoreni funkce a pak dosazeni params
    printf("\n  create func w/out params: ");
    func = ht_addFunction(&table, "funcCreateWithParams2", INTEGER);
    if (func == NULL) { // chck create
        printf("bad - cant create\n");
        allTestsPass = false;
    } else {
        tFunctionData * data = func->data;
        if (data == NULL) { // check data
            printf("bad - no data\n");
            allTestsPass = false;
        } else {
            if (data->params != NULL) { // check params list
                printf("bad - already params\n");
                allTestsPass = false;
            }
            else {

                if ( ! ht_setFuncParams(&table, "funcCreateWithParams2", listParams)) {
                    printf("bad - cant set params\n");
                    allTestsPass = false;
                }
                else {
                    // chcek params val
                    if (data->params->item == NULL) { // check first
                        printf("bad - first not init\n");
                        allTestsPass = false;
                    } else {
                        if (strcmp(data->params->item->name, "param1") != 0) {
                            printf("bad - not first param\n");
                            allTestsPass = false;
                        } else {
                            printf("ok - set params\n");
                        }
                    }
                }
            }
        }
    }



    printf("\n\nCheck function params:\n");
    // Kontrola jestli je tato funkce
    // listParams ok params pro funcCreateWithParams
    printf("\n  is functions same: ");
    if (ht_isFunction(&table, "funcCreateWithParams", INTEGER, listParams) != HT_FUNC_OK) {
        printf("bad - not same\n");
        allTestsPass = false;
    } else {
        printf("ok - same\n");
    }

    printf("\n  is func bad datatype: ");
    if (ht_isFunction(&table, "funcCreateWithParams", DOUBLE, listParams) != HT_FUNC_BAD_DATATYPE) {
        printf("bad\n");
        allTestsPass = false;
    } else {
        printf("ok\n");
    }

    printf("\n  is func not exist: ");
    if (ht_isFunction(&table, "funcCreateWithParamssfasdf", DOUBLE, listParams) != HT_FUNC_NOT_FOUND) {
        printf("bad\n");
        allTestsPass = false;
    } else {
        printf("ok\n");
    }


    // no params jen init
    tFunctionParams  * listInit;
    st_init(&listInit); // init params
    printf("\n  is func init param: ");
    if (ht_isFunction(&table, "funcCreateWithParams", INTEGER, listInit) == HT_FUNC_BAD_PARAMS) {
        printf("ok - not same params\n");
    } else {
        printf("bad - not working\n");
        allTestsPass = false;
    }

    // jinaci params

    // Jen jeden
    st_addParam(listInit, "param1", DOUBLE);
    printf("\n  is func init param: ");
    if (ht_isFunction(&table, "funcCreateWithParams", INTEGER, listInit) == HT_FUNC_BAD_PARAMS) {
        printf("ok - not same\n");
    } else {
        printf("bad - not working\n");
        allTestsPass = false;
    }

    // stejny pocet jinaci params type
    if ( ! st_addParam(listInit, "param2", DOUBLE)) {
        printf("\ncant add param\n");
        allTestsPass = false;
    }
    printf("\n  is func diferent data type: ");
    if (ht_isFunction(&table, "funcCreateWithParams", INTEGER, listInit) == HT_FUNC_BAD_PARAMS) {
        printf("ok - diferent datatype\n");
    } else {
        printf("bad - not working\n");
        allTestsPass = false;
    }
    st_clear(listInit);

    // dva stejne jen nove vytvorene
    st_init(&listInit);
    st_addParam(listInit, "param1", DOUBLE);
    st_addParam(listInit, "param2", INTEGER);
    printf("\n  is fun same: ");
    if (ht_isFunction(&table, "funcCreateWithParams", INTEGER, listInit) == HT_FUNC_BAD_PARAMS) {
        printf("bad - not working\n");
        allTestsPass = false;
    } else {
        printf("ok - same\n");
    }

    // v b vice jinak stejne
    st_addParam(listInit, "param3", INTEGER);
    printf("\n  is func B more: ");
    if (ht_isFunction(&table, "funcCreateWithParams", INTEGER, listInit) != HT_FUNC_BAD_PARAMS) {
        printf("bad - not working\n");
        allTestsPass = false;
    } else {
        printf("ok - not same\n");
    }

    //show_ht_table(&table, false);

    // je vse definovane?
    if ( ! ht_addVariable(ht_getTableFor(&table, "funcCreateWithParams"), "var1", INTEGER)) {
        printf("\ncant add varialbe\n");
        allTestsPass = false;
    }


    printf("\n\nDefinovane vse: \n");

    printf("\n  All definovane: ");
    if (ht_isAllDefined(&table)) {
        printf("bad - not working\n");
        allTestsPass = false;
    } else {
        printf("ok - not all defined\n");
    }

    // dodefinovani funkci
    ht_setFuncDefined(&table, "funcCreateWithParams");
    ht_setFuncDefined(&table, "funcCreateWithParams2");
    printf("\n  All definovane not var: ");
    if (ht_isAllDefined(&table)) {
        printf("bad - not working\n");
        allTestsPass = false;
    } else {
        printf("ok - not all defined\n");
    }


    //
    // Pridavani pomoci hodnoty
    //

    printf("\n\nPridavani pomoci hodnoty:\n");


    //add var from value
    if ( ! ht_addVariable(ht_getTableFor(&table, "funcCreateWithParams"), "var2I", INTEGER)) {
        printf("\ncant add varialbe\n");
        allTestsPass = false;
    }

    printf("\n  add var and set val: ");
    int intVal = 2;
    if (ht_setVarValueInt(ht_getTableFor(&table, "funcCreateWithParams"), "var2I", intVal) != HT_VAR_OK) {
        printf("bad - not working\n");
        allTestsPass = false;
    } else {
        printf("ok - setted\n");
    }


    printf("\n  set bad datatype: ");
    double dblVal = 2.0;
    if (ht_setVarValueDouble(ht_getTableFor(&table, "funcCreateWithParams"), "var2I", dblVal) == HT_VAR_BAD_DATATYPE) {
        printf("ok - bad datatype\n");
    } else {
        printf("bad - not working\n");
        allTestsPass = false;
    }


    // double
    if ( ! ht_addVariable(ht_getTableFor(&table, "funcCreateWithParams"), "var2D", DOUBLE)) {
        printf("\ncant add varialbe\n");
        allTestsPass = false;
    }
    printf("\n  add var double: ");
    double dblVal2 = 2.0;
    if (ht_setVarValueDouble(ht_getTableFor(&table, "funcCreateWithParams"), "var2D", dblVal2) != HT_VAR_OK) {
        printf("bad - not working\n");
        allTestsPass = false;
    } else {
        printf("ok\n");
    }


    // string
    if ( ! ht_addVariable(ht_getTableFor(&table, "funcCreateWithParams"), "var2S", STRING)) {
        printf("\ncant add varialbe\n");
        allTestsPass = false;
    }
    printf("\n  add var string: ");
    const char * valrS1 = "test value";
    if (ht_setVarValueString(ht_getTableFor(&table, "funcCreateWithParams"), "var2S", valrS1) != HT_VAR_OK) {
        printf("bad - not working\n");
        allTestsPass = false;
    } else {
        printf("ok\n");
    }


    if ( ! ht_addVariable(ht_getTableFor(&table, "funcCreateWithParams"), "var2S2", STRING)) {
        printf("\ncant add varialbe\n");
    }
    printf("\n  add var string2: ");
    char * valrS2 = malloc(sizeof(char) * 5);
    strcpy(valrS2, "test");
    if (ht_setVarValueString(ht_getTableFor(&table, "funcCreateWithParams"), "var2S2", valrS2) != HT_VAR_OK) {
        printf("bad - not working\n");
        allTestsPass = false;
    } else {
        printf("ok\n");
    }




    // dodefinovani promennych
    int * intCislo = malloc(sizeof(int));
    *intCislo = 10;
    ht_setVarValue(ht_getTableFor(&table, "funcCreateWithParams"), "var1", intCislo);


    printf("\nAll definovane: ");
    if ( ! ht_isAllDefined(&table)) {
        printf("bad - not working\n");
        allTestsPass = false;
    } else {
        printf("ok - all defined\n");
    }



    // Hledani
    printf("\nHledani polozek: \n");
    printf("\n  Find existing item: ");
    tItem * resItem = ht_getVarForFunc(&table, "funcCreateWithParams", "var1");
    if (resItem == NULL) {
        printf("bad - not working");
        allTestsPass = false;
    } else {
        printf("ok - found");
    }
    printf("\n");

    printf("\n  Find existing somever else item: ");
    resItem = ht_getVarForFunc(&table, "funcCreateWithParams", "testVar1");
    if (resItem == NULL) {
        printf("ok - not found");
    } else {
        printf("bad - working bad");
        allTestsPass = false;
    }
    printf("\n");

    printf("\n  Find non existing item: ");
    resItem = ht_getVarForFunc(&table, "funcCreateWithParams", "NONexsitSf");
    if (resItem == NULL) {
        printf("ok - not found");
    } else {
        printf("bad - working bad");
        allTestsPass = false;
    }

    printf("\n");


    // ziskani hodnot
    //ht_getVarValueIntItem();
    printf("\nKontrola hodnot:\n");

    printf("\n  Integer check: ");
    resItem = ht_getVarForFunc(&table, "funcCreateWithParams", "var1");
    int * valIRet = ht_getVarValueIntItem(resItem);
    if (valIRet != NULL) {
        if (*valIRet == 10) {
            printf("ok - value ok\n");
        } else {
            printf("bad - vron value\n");
            allTestsPass = false;
        }
    } else {
        printf("bad - not value\n");
        allTestsPass = false;
    }

    printf("\n  Integer check bad: ");
    double * valDRet = ht_getVarValueDoubleItem(resItem);
    if (valDRet != NULL) {
        if (*valDRet == 10.0) {
            printf("bad - value ok\n");
            allTestsPass = false;
        } else {
            printf("bad - vron value\n");
            allTestsPass = false;
        }
    } else {
        printf("ok - not value\n");
    }


    printf("\n  Double check: ");
    resItem = ht_getVarForFunc(&table, "funcCreateWithParams", "var2D");
    valDRet = ht_getVarValueDoubleItem(resItem);
    if (valDRet != NULL) {
        if (*valDRet == 2.0) {
            printf("ok - value ok\n");
        } else {
            printf("bad - vron value\n");
            allTestsPass = false;
        }
    } else {
        printf("bad - not value\n");
        allTestsPass = false;
    }


    printf("\n  String check: ");
    resItem = ht_getVarForFunc(&table, "funcCreateWithParams", "var2S2");
    char * varSRet = ht_getVarValueStringItem(resItem);
    if (varSRet != NULL) {
        if (strcmp(varSRet, "test") == 0) {
            printf("ok - value ok\n");
        } else {
            printf("bad - vron value\n");
            allTestsPass = false;
        }
    } else {
        printf("bad - not value\n");
        allTestsPass = false;
    }



    printf("\nNastaveni hodnot pomoci nazvu funkce:\n");
    printf("\n  Int set exist: ");
    int res;
    if ((res = ht_setVarValueIntForFunc(&table, "funcCreateWithParams", "var2I", 123)) != HT_VAR_OK) {
        printf("bad - not working %d\n", res);
        allTestsPass = false;
    } else {
        printf("ok - setted\n");
    }

    printf("\n  Int set non exist: ");
    if ((ht_setVarValueIntForFunc(&table, "funcCreateWithParams", "var2Sds", 1233)) == HT_VAR_NOT_FOUD) {
        printf("ok - not found\n");
    } else {
        printf("bad - setted\n");
        allTestsPass = false;
    }

    printf("\n  Int set exist but not in func: ");
    if ((ht_setVarValueIntForFunc(&table, "testFunkce", "var2I", 123)) == HT_VAR_NOT_FOUD) {
        printf("ok - not found\n");
    } else {
        printf("bad - setted\n");
        allTestsPass = false;
    }

    printf("\n  Double set exist: ");
    if ((res = ht_setVarValueDoubleForFunc(&table, "funcCreateWithParams", "var2D", 1.23)) != HT_VAR_OK) {
        printf("bad - not working %d\n", res);
        allTestsPass = false;
    } else {
        printf("ok - setted\n");
    }

    printf("\n  Double set non exist: ");
    if ((ht_setVarValueDoubleForFunc(&table, "funcCreateWithParams", "var2Dsdas", 1.23)) == HT_VAR_NOT_FOUD) {
        printf("ok - not found\n");
    } else {
        printf("bad - setted\n");
        allTestsPass = false;
    }

    printf("\n  Double set exist but not in func: ");
    if ((ht_setVarValueDoubleForFunc(&table, "testFunkce", "var2D", 1.23)) == HT_VAR_NOT_FOUD) {
        printf("ok - not found\n");
    } else {
        printf("bad - setted\n");
        allTestsPass = false;
    }


    printf("\n  String set exist: ");
    if ((res = ht_setVarValueStringForFunc(&table, "funcCreateWithParams", "var2S", "test change")) != HT_VAR_OK) {
        printf("bad - not working %d\n", res);
        allTestsPass = false;
    } else {
        printf("ok - setted\n");
    }

    printf("\n  String set non exist: ");
    if ((ht_setVarValueStringForFunc(&table, "funcCreateWithParams", "var2Sds", "test change")) == HT_VAR_NOT_FOUD) {
        printf("ok - not found\n");
    } else {
        printf("bad - setted\n");
        allTestsPass = false;
    }

    printf("\n  String set exist but not in func: ");
    if ((ht_setVarValueStringForFunc(&table, "testFunkce", "var2Sds", "test change")) == HT_VAR_NOT_FOUD) {
        printf("ok - not found\n");
    } else {
        printf("bad - setted\n");
        allTestsPass = false;
    }


    show_ht_table(&table, false);
    printf("\n clear\n");
    ht_clearAll(&table);
    show_ht_table(&table, false);


    if (allTestsPass) {
        printf("\n\nVse ok!!\n");
    } else {
        printf("\n\nSome test wrong!!\n");
    }

*/
    return 0;
}