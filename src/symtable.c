//
// Created by root on 11/23/17.
//

#include "symtable.h"

int ht_hashCode(tName name) {
    // Zdroj Martin


    //prime number
    const int MagicalNumber = 37;
    int hash = 0;

    for(unsigned int i = 0; i < strlen(name); i++){
        hash = hash ^ name[i]; // XOR every char of key
    }

    hash = MagicalNumber * hash % MAX_HTTABLE_SIZE; // multiply by magical number, and make it index for the table

    return hash;
}

void ht_init(ht_table *prtTable) {

    if (prtTable != NULL) {

        int i;
        // for each radek
        for ( i = 0; i < MAX_HTTABLE_SIZE; ++i) {
            (*prtTable)[i] = NULL;
        }
    }
}

tItem *ht_search(ht_table *prtTable, tName name) {

    if(prtTable != NULL) { // existuje table?

        int index = ht_hashCode(name); // search

        tItem * tmp = (*prtTable)[index];

        while(tmp != NULL) {

            // Nasli jsme dany klic?
            if(!strcmp(tmp->name, name)) {
                return tmp;
            }
            tmp = tmp->nextItem; // posun na dalsi synonymum
        }
    }

    return NULL;
}

tItem *ht_insert(ht_table *prtTable, tName name) {

    if(prtTable != NULL) { // existuje table?

        tItem *tmp = ht_search(prtTable, name); // existuje uz?

        if(tmp != NULL) { // Existuje uz?
            return tmp;
        }

        // Tvorba nove polozky
        tmp = malloc(sizeof(tItem));

        if(tmp == NULL) {
            // todo INTERNAL ERROR
            return NULL;
        }

        int len = strlen(name); // delka pro klic
        int index = ht_hashCode(name);

        tmp->name = malloc(len + 1); // alokace pro klic

        if(tmp->name == NULL) {
            free(tmp);
            // todo INTERNAL ERROR
            return NULL;
        }

        strcpy(tmp->name, name);

        // Prevazani
        tmp->nextItem = (*prtTable)[index];
        (*prtTable)[index] = tmp;

        return tmp;
    }
    return NULL;
}

void ht_delete(ht_table *prtTable, tName name) {

    if(prtTable != NULL) { // existuje table?

        int index = ht_hashCode(name);
        tItem *tmp1 = (*prtTable)[index];
        tItem *tmp2 = (*prtTable)[index];

        if(tmp1 == NULL) { // existuje polozka?
            return;
        }

        if( ! strcmp(tmp1->name, name)) { // Je to aktualni polozka?

            (*prtTable)[index] = tmp1->nextItem; // odmazani aktualni
            ht_deleteItem(tmp1);
            return;
        }

        while(tmp1->nextItem != NULL) { // neni to prvni, musime prochazet synonyma

            tmp2 = tmp1;
            tmp1 = tmp1->nextItem; // next

            if ( ! strcmp(tmp1->name, name)) {
                tmp2->nextItem = tmp1->nextItem;
                ht_deleteItem(tmp1);
                return;
            }
            tmp2 = tmp1;
        }
    }
}

void ht_deleteItem(tItem *item) {

    if (item != NULL) { // existuje?

        if (item->name == NULL) {
            // todo Sem by se nemelo spravne dostat
            debug_log("HashTable", __FUNCTION__, "Jeden Item byl pouzit dvakrat. Zkontroluj si odkazovani.");
            //free(item);
            return;
        }

        // clear next
            // nemusime se starat

        // clear data
        if (item->type == function) {

            tFunctionData *data = (tFunctionData *) item->data;

            ht_clearAll(&data->sTable);

            st_clear(data->params);

            free(data);

        } else if (item->type == variable) {
            if (item->data != NULL) { // Jsou nejaka data
                tVariableData *data = (tVariableData *) item->data;
                if (data->data != NULL) {
                    free(data->data);
                }
                free(data);
            }
        }

        // clear name
        free(item->name);

        // clear it self
        free(item);

    }
}

void ht_clearAll(ht_table *prtTable) {

    if(prtTable != NULL) { // existuje table?

        tItem *tmp1;
        tItem *tmp2;

        // for each row
        for (int i = 0; i < MAX_HTTABLE_SIZE; i++) {

            tmp1 = (*prtTable)[i];
            // while synonymum
            while (tmp1 != NULL) {
                tmp2 = tmp1->nextItem; // next
                ht_deleteItem(tmp1);
                tmp1 = tmp2;
            }
            (*prtTable)[i] = NULL; // clear polozku
        }
    }
}

tItem *ht_addVariable(ht_table *ptrTable, char *name, tokenTypes dataType) {

    // Existuje tabulka?
    if (ptrTable == NULL) {
        return NULL; // neni tabulka
    }

    // Existuje uz nejaky zaznam?
    if (ht_search(ptrTable, name) != NULL) {
        return NULL; // existuje
    }

    tItem * item = ht_insert(ptrTable, name);

    if (item == NULL) {
        //todo internal error
        return NULL;
    }

    item->dataType = dataType;
    item->data = NULL;
    item->nextItem = NULL;
    item->type = variable;

    return item;
}


int ht_setVarValueItem(tItem *item, void *data) {

    if (item == NULL) {
        return HT_VAR_NOT_FOUD; // neexistuje
    }

    // Neni va?
    if (item->type != variable) {
        return HT_VAR_NOT_VAR;
    }

    if (item->type == variable) {
        tVariableData * dataItem = item->data;

        if (dataItem == NULL) {
            dataItem = malloc(sizeof(tVariableData));
            if (dataItem == NULL) {
                // todo interni error
                return HT_ITERNAL_ERROR;
            }
        }
        else {
            if (dataItem->data != NULL) {
                // uvolnime to co tu bylo predtim
                free(dataItem->data);
            }
        }

        dataItem->data = data;
        item->data = dataItem;

        return HT_VAR_OK; // ok
    }


    return HT_ITERNAL_ERROR;
}


int ht_setVarValue(ht_table *ptrTable, char *name, void *data) {

    // Existuje tabulka?
    if (ptrTable == NULL) {
        return HT_NULL_TABLE; // neni tabulka
    }

    // Existuje uz nejaky zaznam?
    tItem * item = ht_search(ptrTable, name);

    return ht_setVarValueItem(item, data);
}



int ht_setVarValueInt(ht_table *ptrTable, char *name, int data) {

    // Existuje tabulka?
    if (ptrTable == NULL) {
        return HT_NULL_TABLE; // neni tabulka
    }

    // Existuje uz nejaky zaznam?
    tItem * item = ht_search(ptrTable, name);
    if (item == NULL) {
        return HT_VAR_NOT_FOUD; // neexistuje
    }

    // Neni va?
    if (item->type != variable) {
        return HT_VAR_NOT_VAR;
    }

    // checka datatype
    if (item->dataType != INTEGER) {
        return HT_VAR_BAD_DATATYPE;
    }

    int * valInt = malloc(sizeof(int));
    if (valInt == NULL) {
        // todo internal erro malloc
        return HT_ITERNAL_ERROR;
    }

    // zkopirovani dat
    memcpy(valInt, &data, sizeof(int));

    return ht_setVarValue(ptrTable, name, valInt);
}

int ht_setVarValueIntForFunc(ht_table *ptrTable, char *funcName, char *varName, int data) {

    // Existuje tabulka?
    if (ptrTable == NULL) {
        return HT_NULL_TABLE; // neni tabulka
    }

    tItem * item;

    // Existuje funkce?
    tItem * func = ht_search(ptrTable, funcName);
    if (func != NULL) {

        // search ve funkci
        item = ht_search(ht_getTableFor(ptrTable, funcName), varName);
        if (item != NULL) {
            if (item->type != variable) {
                item = NULL;
            }
        }
    }

    // Search v globalni tab symbolu
    if (item == NULL) {
        item = ht_search(ptrTable, varName);
        if (item == NULL) {
            return HT_VAR_NOT_FOUD; // neexistuje
        }
    }

    // Neni va?
    if (item->type != variable) {
        return HT_VAR_NOT_VAR;
    }

    // checka datatype
    if (item->dataType != INTEGER) {
        return HT_VAR_BAD_DATATYPE;
    }

    int * valInt = malloc(sizeof(int));
    if (valInt == NULL) {
        // todo internal erro malloc
        return HT_ITERNAL_ERROR;
    }

    // zkopirovani dat
    memcpy(valInt, &data, sizeof(int));

    return ht_setVarValueItem(item, valInt);
}

int ht_setVarValueDouble(ht_table *ptrTable, char *name, double data) {

    // Existuje tabulka?
    if (ptrTable == NULL) {
        return HT_NULL_TABLE; // neni tabulka
    }

    // Existuje uz nejaky zaznam?
    tItem * item = ht_search(ptrTable, name);
    if (item == NULL) {
        return HT_VAR_NOT_FOUD; // neexistuje
    }

    // Neni va?
    if (item->type != variable) {
        return HT_VAR_NOT_VAR;
    }

    // checka datatype
    if (item->dataType != DOUBLE) {
        return HT_VAR_BAD_DATATYPE;
    }

    double * valDouble = malloc(sizeof(double));
    if (valDouble == NULL) {
        // todo internal erro malloc
        return HT_ITERNAL_ERROR;
    }

    // zkopirovani dat
    memcpy(valDouble, &data, sizeof(double));

    return ht_setVarValue(ptrTable, name, valDouble);
}


int ht_setVarValueDoubleForFunc(ht_table *ptrTable, char *funcName, char *varName, double data) {

    // Existuje tabulka?
    if (ptrTable == NULL) {
        return HT_NULL_TABLE; // neni tabulka
    }

    tItem * item;

    // Existuje funkce?
    tItem * func = ht_search(ptrTable, funcName);
    if (func != NULL) {

        // search ve funkci
        item = ht_search(ht_getTableFor(ptrTable, funcName), varName);
        if (item != NULL) {
            if (item->type != variable) {
                item = NULL;
            }
        }
    }

    // Search v globalni tab symbolu
    if (item == NULL) {
        item = ht_search(ptrTable, varName);
        if (item == NULL) {
            return HT_VAR_NOT_FOUD; // neexistuje
        }
    }

    // Neni va?
    if (item->type != variable) {
        return HT_VAR_NOT_VAR;
    }

    // checka datatype
    if (item->dataType != DOUBLE) {
        return HT_VAR_BAD_DATATYPE;
    }

    double * valDouble = malloc(sizeof(double));
    if (valDouble == NULL) {
        // todo internal erro malloc
        return HT_ITERNAL_ERROR;
    }

    // zkopirovani dat
    memcpy(valDouble, &data, sizeof(double));

    return ht_setVarValueItem(item, valDouble);
}


int ht_setVarValueString(ht_table *ptrTable, char *name, char *data) {

    // Existuje tabulka?
    if (ptrTable == NULL) {
        return HT_NULL_TABLE; // neni tabulka
    }

    // Existuje uz nejaky zaznam?
    tItem * item = ht_search(ptrTable, name);
    if (item == NULL) {
        return HT_VAR_NOT_FOUD; // neexistuje
    }

    // Neni va?
    if (item->type != variable) {
        return HT_VAR_NOT_VAR;
    }

    // checka datatype
    if (item->dataType != STRING) {
        return HT_VAR_BAD_DATATYPE;
    }

    char * valStr = malloc(sizeof(char) * strlen(data));
    if (valStr == NULL) {
        // todo internal erro malloc
        return HT_ITERNAL_ERROR;
    }

    // zkopirovani dat
    strcpy(valStr, data);

    return ht_setVarValue(ptrTable, name, valStr);
}


int ht_setVarValueStringForFunc(ht_table *ptrTable, char *funcName, char *varName, char *data) {

    // Existuje tabulka?
    if (ptrTable == NULL) {
        return HT_NULL_TABLE; // neni tabulka
    }

    tItem * item;

    // Existuje funkce?
    tItem * func = ht_search(ptrTable, funcName);
    if (func != NULL) {

        // search ve funkci
        item = ht_search(ht_getTableFor(ptrTable, funcName), varName);
        if (item != NULL) {
            if (item->type != variable) {
                item = NULL;
            }
        }
    }

    // Search v globalni tab symbolu
    if (item == NULL) {
        item = ht_search(ptrTable, varName);
        if (item == NULL) {
            return HT_VAR_NOT_FOUD; // neexistuje
        }
    }

    // Neni va?
    if (item->type != variable) {
        return HT_VAR_NOT_VAR;
    }

    // checka datatype
    if (item->dataType != STRING) {
        return HT_VAR_BAD_DATATYPE;
    }

    char * valStr = malloc(sizeof(char) * strlen(data));
    if (valStr == NULL) {
        // todo internal erro malloc
        return HT_ITERNAL_ERROR;
    }

    // zkopirovani dat
    strcpy(valStr, data);

    return ht_setVarValueItem(item, valStr);
}



void * ht_getVarValue(ht_table *ptrTable, char *name) {

    // Existuje tabulka?
    if (ptrTable == NULL) {
        return NULL; // neni tabulka
    }

    // Existuje uz nejaky zaznam?
    tItem * item = ht_search(ptrTable, name);
    if (item == NULL) {
        return NULL; // neexistuje
    }

    tVariableData * data = (tVariableData*) item->data;
    if (data != NULL) {
        return data->data;
    }

    return NULL;
}

int ht_isVarExist(ht_table *ptrTable, char *name) {

    // exist?
    if (ptrTable == NULL) {
        return HT_NULL_TABLE;
    }

    tItem * item = ht_search(ptrTable, name);
    if (item == NULL) {
        return HT_VAR_NOT_FOUD; // Not exist var
    }

    // not var?
    if (item->type != variable) {
        return HT_VAR_NOT_VAR;
    }

    return HT_VAR_OK; // exist
}

bool ht_isVarDefinedItem(tItem * item) {

    if (item == NULL) {
        return false; // Not exist var
    }

    // funkce?
    if (item->type == function) {
        return false;
    }

    // Jsou tam data?
    if (item->data == NULL) {
        return false;
    }

    return true;
}


bool ht_isVarDefined(ht_table *ptrTable, char *name) {
    // exist?
    if (ptrTable == NULL) {
        return false;
    }

    tItem * item = ht_search(ptrTable, name);

    return ht_isVarDefinedItem(item);
}

tItem *ht_addFunction(ht_table *ptrTable, char *name, tokenTypes dataType) {

    if (ptrTable == NULL) {
        return NULL;
    }

    // HT_VAR_NOT_FOUD = neexistuje tento prvek
    if (ht_isVarExist(ptrTable, name) != HT_VAR_NOT_FOUD) {
        return NULL;
    }

    // Zalozeni nove funkce
    tItem * fce = ht_insert(ptrTable, name);
    if (fce == NULL) {
        return NULL;
    }

    fce->type = function;
    fce->dataType = dataType;
    fce->nextItem = NULL;

    // tabulka symbolu
    tFunctionData * data = malloc(sizeof(tFunctionData));
    if (data == NULL) {
        // todo internal erro
        return NULL;
    }

    data->params = NULL;
    data->defined = false;
    ht_init(&data->sTable);

    fce->data = data;

    return fce;
}

tItem *ht_addFunctionWithParams(ht_table *ptrTable, char *name, tokenTypes dataType, tFunctionParams *params) {

    tItem * fce = ht_addFunction(ptrTable, name, dataType);
    if (fce == NULL) {
        return NULL;
    }

    // Pridani parametru
    tFunctionData * data = (tFunctionData*) fce->data;
    data->params = params;

    return fce;
}



ht_table *ht_getTableFor(ht_table *ptrTable, char *functionName) {

    // exist?
    if (ptrTable == NULL) {
        return NULL;
    }

    tItem * item = ht_search(ptrTable, functionName);
    if (item == NULL) {
        return NULL; // Not exist var
    }

    // not funkce?
    if (item->type != function) {
        return NULL;
    }

    tFunctionData * data = (tFunctionData *) item->data;

    if (data == NULL) {
        return NULL;
    }

    return &data->sTable;
}

bool ht_isFuncDefinedItem(tItem *funkce) {

    if (funkce != NULL) {
        if (funkce->type == function) {
            return ((tFunctionData *) (funkce->data))->defined;
        }
    }
    return false;
}

bool ht_isFuncDefined(ht_table *ptrTable, char *functionName) {

    // exist?
    if (ptrTable == NULL) {
        return false;
    }

    tItem * item = ht_search(ptrTable, functionName);
    if (item == NULL) {
        return false; // Not exist var
    }

    return ht_isFuncDefinedItem(item);
}

void ht_setFuncDefinedItem(tItem *funkce) {
    if (funkce != NULL) {
        if (funkce->type == function) {
            ((tFunctionData *) (funkce->data))->defined = true;
        }
    }
}

void ht_setFuncDefined(ht_table *ptrTable, char *functionName) {

    // exist?
    if (ptrTable == NULL) {
        return ;
    }

    tItem * item = ht_search(ptrTable, functionName);
    if (item == NULL) {
        return ; // Not exist var
    }

    ht_setFuncDefinedItem(item);
}

bool ht_isAllDefined(ht_table *ptrTable) {

    //existuje?
    if (ptrTable == NULL) {
        return false;
    }

    tItem *tmp1;

    // for each row
    for (int i = 0; i < MAX_HTTABLE_SIZE; i++) {

        tmp1 = (*ptrTable)[i];
        // while synonymum
        while (tmp1 != NULL) {
            if (tmp1->type == variable) {
                if ( ! ht_isVarDefinedItem(tmp1)) return false;
            }
            else if (tmp1->type == function) {
                if ( ! ht_isFuncDefinedWithTable(ptrTable, tmp1->name)) return false;
            }

            tmp1 = tmp1->nextItem; // next
        }
    }

    return true;
}

bool ht_isFuncDefinedWithTable(ht_table *ptrTable, char *functionName) {
    // exist?
    if (ptrTable == NULL) {
        return false;
    }

    tItem * item = ht_search(ptrTable, functionName);
    if (item == NULL) {
        return false; // Not exist var
    }

    //          je definovana fce?          je definovana tabulka fce?
    return ht_isFuncDefinedItem(item) && ht_isAllDefined(ht_getTableFor(ptrTable, functionName));
}

void st_init(tFunctionParams **list) {

    tFunctionParams * tmp = malloc(sizeof(tFunctionParams));
    if (tmp == NULL) {
        // todo internal error malloc
        return;
    }
    *list = tmp;

    (*list)->nextParam = NULL;
    (*list)->item = NULL;
}

void st_clear(tFunctionParams *list) {

    if (list != NULL) {
        while (list != NULL) {
            st_removeLast(&list);
        }
    }
}

bool st_add(tFunctionParams *list, tItem *item) {
    if (list != NULL) {

        // Neni jeste item [prvni]
        if (list->item == NULL) {

            list->item = item;
            list->nextParam = NULL;
            return true;
        }

        // Uz tam je item s timto nazvem?
        if (st_isExist(list, item->name)) {
            return false;
        }

        // Je uz item
        tFunctionParams * tmp = list->nextParam;
        tFunctionParams * tmpPrew = list;

        // az na konec
        while (tmp != NULL) {
            tmpPrew = tmp;
            tmp = tmp->nextParam;
        }

        tmp = malloc(sizeof(tFunctionParams));
        if (tmp == NULL) {
            // todo internal erro
            return false;
        }

        // provazani
        tmpPrew->nextParam = tmp;

        tmp->item = item;
        tmp->nextParam = NULL;

        return true;
    }

    return false;
}

void st_removeLast(tFunctionParams **list) {

    if (list == NULL) {
        return;
    }

    if ((*list) != NULL) {

        // Ounly one
        if ((*list)->nextParam == NULL) {
            ht_deleteItem((*list)->item);
            free(*list);
            (*list) = NULL;
            return;
        }

        // na konec
        tFunctionParams * current = (*list);
        while (current->nextParam->nextParam != NULL) {
            current = current->nextParam;
        }


        ht_deleteItem(current->nextParam->item);
        free(current->nextParam);
        current->nextParam = NULL;
        return;
    }
}

bool st_isSame(tFunctionParams *listA, tFunctionParams *listB) {

    if ((listA == NULL) && (listB == NULL)) {
        return true;
    }


    tFunctionParams * tmpA = listA;
    tFunctionParams * tmpB = listB;

    // prochazime listA a kontrolujeme proti listB
    // pokud projde az na konec zeptame se jestli
    // je jeste neco v listB
    while (tmpA != NULL) {

        // listB uz dosel
        if (tmpB == NULL) {
            return false;
        }

        //compare
        if (tmpA->item == NULL) { // not setted yet
            if (tmpB->item != NULL) {
                return false;
            }
        }

        if (tmpA->item != NULL) {
            if (tmpB->item == NULL) { // neni nastaven
                return false;
            }

            // compare itemu
            // name
            if (strcmp(tmpA->item->name, tmpB->item->name) != 0) {
                return false;
            }
            // datatype
            if (tmpA->item->dataType != tmpB->item->dataType) {
                return false;
            }
        }

        // next
        tmpA = tmpA->nextParam;
        tmpB = tmpB->nextParam;
    }

    // v b jeste neco je
    if (tmpB != NULL) {
        return false;
    }

    return true;
}

tItem *ht_creatItem(tName name, tokenTypes dataType) {

    // Tvorba nove polozky
    tItem * tmp = malloc(sizeof(tItem));

    if(tmp == NULL) {
        // todo INTERNAL ERROR
        return NULL;
    }

    size_t len = strlen(name); // delka pro klic

    tmp->name = malloc(len + 1); // alokace pro klic

    if(tmp->name == NULL) {
        free(tmp);
        // todo INTERNAL ERROR
        return NULL;
    }

    strcpy(tmp->name, name);
    tmp->dataType = dataType;

    return tmp;
}

bool ht_setFuncParamsItem(tItem * item, tFunctionParams *params) {

    if (item == NULL) {
        return false;
    }
    // not function
    if (item->type != function) {
        return false;
    }

    tFunctionData * data = (tFunctionData*) item->data;
    if (data == NULL) {
        return false;
    }

    // Uz nejake jsou?
    if (data->params != NULL) {
        // clear old params
        st_clear(data->params);
    }

    data->params = params;

    return true;
}

bool ht_setFuncParams(ht_table *ptrTable, char *functionName, tFunctionParams * params) {
    // exist?
    if (ptrTable == NULL) {
        return false;
    }

    tItem * func = ht_search(ptrTable, functionName);
    if (func == NULL) {
        return false; // Not exist var
    }

    return ht_setFuncParamsItem(func, params);
}

bool st_addParam(tFunctionParams *list, char *name, tokenTypes dataType) {

    // create param
    tItem * param = ht_creatItem(name, dataType);
    if (param == NULL) {
        return false;
    }

    param->type = parametr;

    // not add param
    if ( ! st_add(list, param)) {
        return false;
    }

    return true;
}

bool st_isExist(tFunctionParams *list, char *name) {

    if (list == NULL) {
        return false;
    }

    tFunctionParams * tmp = list;

    while (tmp != NULL) {

        if (tmp->item != NULL) {
            if (strcmp(tmp->item->name, name) == 0) {
                return true;
            }
        }
        tmp = tmp->nextParam;
    }

    return false;
}

int ht_isFunctionItem(tItem * func, char *name, tokenTypes retType, tFunctionParams *params) {

    if (func == NULL) {
        return HT_FUNC_NOT_FOUND;
    }

    // not je funkci?
    if (func->type != function) {
        return HT_FUNC_NOT_FUNC;
    }

    // not shoduje jmeno?
    if (strcmp(func->name, name) != 0) {
        return HT_FUNC_BAD_NAME;
    }

    // not return type
    if (func->dataType != retType) {
        return HT_FUNC_BAD_DATATYPE;
    }

    tFunctionData * data = (tFunctionData*) func->data;
    if ( ! st_isSame(data->params, params)) {
        return HT_FUNC_BAD_PARAMS;
    }

    return HT_FUNC_OK; // ok - all same
}


int ht_isFunction(ht_table *ptrTable, char *name, tokenTypes retType, tFunctionParams *params) {

    if (ptrTable == NULL) {
        return HT_NULL_TABLE;
    }

    tItem * func = ht_search(ptrTable, name);

    return ht_isFunctionItem(func, name, retType, params);
}

tokenTypes ht_getDataTypeItem(tItem *item) {
    if (item != NULL) {
        return item->dataType;
    }
    return IDENTIFIER;
}

tokenTypes ht_getDataType(ht_table *ptrTable, char *name) {

    return ht_getDataTypeItem(ht_search(ptrTable, name));
}

int *ht_getVarValueIntItem(tItem *item) {
    if (item != NULL) {
        if (item->dataType == INTEGER) {
            tVariableData * data = item->data;
            if (data != NULL) {
                if (data->data != NULL) {
                    return (int *) data->data;
                }
            }
        }
    }
    return NULL;
}

int *ht_getVarValueInt(ht_table *ptrTable, char *name) {
    return ht_getVarValueIntItem(ht_search(ptrTable, name));
}

double *ht_getVarValueDoubleItem(tItem *item) {
    if (item != NULL) {
        if (item->dataType == DOUBLE) {
            tVariableData * data = item->data;
            if (data != NULL) {
                if (data->data != NULL) {
                    return (double *) data->data;
                }
            }
        }
    }
    return NULL;
}

double *ht_getVarValueDouble(ht_table *ptrTable, char *name) {
    return ht_getVarValueDoubleItem(ht_search(ptrTable, name));
}

char *ht_getVarValueStringItem(tItem *item) {

    if (item != NULL) {
        if (item->dataType == STRING) {
            tVariableData * data = item->data;
            if (data != NULL) {
                if (data->data != NULL) {
                    return (char *) data->data;
                }
            }
        }
    }
    return NULL;
}

char *ht_getVarValueString(ht_table *ptrTable, char *name) {
    return ht_getVarValueStringItem(ht_search(ptrTable, name));
}

tItem *ht_getVarForFunc(ht_table *ptrTable, char *funcName, char *varName) {

    // search tabulku funkce
    tItem * tmp = ht_search(ht_getTableFor(ptrTable, funcName), varName);
    if (tmp != NULL) {
        if (tmp->type == variable) {
            return tmp;
        }
    }

    // search globalni
    tmp = ht_search(ptrTable, varName);
    if (tmp != NULL) {
        if (tmp->type == variable) {
            return tmp;
        }
    }

    return NULL;
}

int ht_isFuncExist(ht_table *ptrTable, char *name) {
    
    // exist?
    if (ptrTable == NULL) {
        return HT_NULL_TABLE;
    }

    tItem * item = ht_search(ptrTable, name);
    if (item == NULL) {
        return HT_FUNC_NOT_FOUND; // Not exist var
    }

    // not var?
    if (item->type != function) {
        return HT_FUNC_NOT_FUNC;
    }

    return HT_FUNC_OK; // exist
}


