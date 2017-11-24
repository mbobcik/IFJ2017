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

tItem *ht_read(ht_table *prtTable, tName name) {

    if(prtTable != NULL) { // existuje table?

        tItem *tmp = ht_search(prtTable, name);

        if(tmp != NULL) {
            return tmp;
        }
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

        // clear name
        free(item->name);
        // clear next
            // nemusime se starat

        // clear data
        if (item->type == function) {

            tFunctionData *data = (tFunctionData *) item->data;

            ht_clearAll(&data->sTable);

            tFunctionParams *param1 = (tFunctionParams *) data->params;
            tFunctionParams *parma2 = param1;
            while (param1 != NULL) {
                parma2 = param1->nextParam;
                ht_deleteParam(param1);
                param1 = parma2;
            }

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

void ht_deleteParam(tFunctionParams *param) {

    // smazani promenne
    ht_deleteItem(param->item);

    param->nextParam = NULL;
    param->firstParam = NULL;

    free(param);
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

int ht_setVarValue(ht_table *ptrTable, char *name, void *data) {

    // Existuje tabulka?
    if (ptrTable == NULL) {
        return 0; // neni tabulka
    }

    // Existuje uz nejaky zaznam?
    tItem * item = ht_search(ptrTable, name);
    if (item == NULL) {
        return -1; // neexistuje
    }

    // Je to funkce?
    if (item->type == function) {
        return -2; // je to funkce
    }

    if (item->type == variable) {
        tVariableData * dataItem = item->data;

        if (dataItem == NULL) {
            dataItem = malloc(sizeof(tVariableData));
            if (dataItem == NULL) {
                // todo interni error
                return -3;
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

        return 1; // ok
    }


    return -3;
}

void *ht_getVarValue(ht_table *ptrTable, char *name) {

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
        return -2;
    }

    tItem * item = ht_search(ptrTable, name);
    if (item == NULL) {
        return 0; // Not exist var
    }

    // funkce?
    if (item->type == function) {
        return -1;
    }

    return 1; // exist
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

    // 0 = neexistuje tento prvek
    if (ht_isVarExist(ptrTable, name) != 0) {
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
                if ( ! ht_isFuncDefinedItem(tmp1)) return false;
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

void st_init(tFunctionParams *list) {

    if (list == NULL) {
        tFunctionParams * tmp = malloc(sizeof(tFunctionParams));
        if (tmp == NULL) {
            // todo internal erro
            return;
        }
        list = tmp;
        list->nextParam = NULL;
        list->firstParam = NULL;
        list->item = NULL;
    }
}

void st_clear(tFunctionParams *list) {

    if (list != NULL) {
        while (list->nextParam != NULL) {
            st_removeLast(list);
        }

        st_removeLast(list); // remove prvniho
    }
}

bool st_add(tFunctionParams *list, tItem *item) {
    if (list != NULL) {

        // Neni jeste item
        if (list->item == NULL) {

            list->item = item;
            list->firstParam = list;
            list->nextParam = NULL;
            return true;
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
        tmp->firstParam = list->firstParam;
        tmp->nextParam = NULL;

        return true;
    }
}

void st_removeLast(tFunctionParams *list) {

    if (list != NULL) {

        // poze jeden?
        if (list->nextParam == NULL) {
            // remove last == first
            // free item
            ht_deleteItem(list->item);
            free(list);
            return;
        }

        tFunctionParams * tmp = list->nextParam;
        tFunctionParams * tmpPrew = list;
        tFunctionParams * tmpPrewPrew = list;
        // az na konec
        while (tmp != NULL) {
            tmpPrewPrew = tmpPrew;
            tmpPrew = tmp;
            tmp = tmp->nextParam;
        }

        tmpPrewPrew->nextParam = NULL;

        // free item
        ht_deleteItem(tmpPrew->item);
        free(tmpPrew);
    }
}

tItem *ht_creatItem(tName name, tokenTypes dataType) {

    // Tvorba nove polozky
    tItem * tmp = malloc(sizeof(tItem));

    if(tmp == NULL) {
        // todo INTERNAL ERROR
        return NULL;
    }

    int len = strlen(name); // delka pro klic

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
