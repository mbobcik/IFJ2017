//
// Created by root on 11/23/17.
//
#include "functions.h"



void debug_log(char *modulName,const char *functionName, char *message) {
    printf("%s.%s: %s\n", modulName, functionName, message);
    fflush(stdout);
}

void show_ht_table(ht_table *table, bool addPadding) {

    if (table != NULL) {

        bool empty = true;
        int cnt = 0;

        if (addPadding) printf("\t\t");
        printf("=============================================\n");
        if (addPadding) printf("\t\t");
        printf("|             Tabulka symbolu               |\n");
        if (addPadding) printf("\t\t");
        printf("=============================================\n");
        for (int i = 0; i < MAX_HTTABLE_SIZE; ++i) {
            tItem * item = (*table)[i];
            show_ht_item(item, addPadding);
            if (item != NULL) {
                empty = false;
                cnt++;
            }
        }
        if (empty) {
            if (addPadding) printf("\t\t");
            printf("|                  EMPTY                    |\n");
        }
        if (addPadding) printf("\t\t");
        printf("=============================================\n");
        if (addPadding) printf("\t\t");
        printf("| Items: %d\n", cnt);
        if (addPadding) printf("\t\t");
        printf("=============================================\n");
    } else {
        if (addPadding) printf("\t\t");
        printf("[NOT TABLE]");
    }
}

void show_ht_item(tItem *item, bool addPadding) {

    if (item != NULL) {

        // je tam synonymum?
        if (item->nextItem != NULL) {
            // odsadime vytup
            tItem * tmp = item;

            while (tmp != NULL) {

                if (tmp->type == function) {
                    // function
                    if (addPadding) printf("\t\t");
                    printf("|\t\t%s %s(", getDataTypeName(tmp->dataType), tmp->name);

                    //parametry
                    tFunctionData * fData = (tFunctionData *) tmp->data;
                    tFunctionParams *param1 =fData->params;
                    while (param1 != NULL) {
                        show_ht_item(param1->item, addPadding);
                        param1 = param1->nextParam;
                    }

                    if (addPadding) printf("\t\t");
                    printf(")");

                    // definovana?
                    if (fData->defined) {
                        printf(" DEF");
                    } else {
                        printf(" NOT DEF");
                    }
                    printf("\n");

                    show_ht_funcTable(tmp);
                }
                else {
                    // variable
                    if (addPadding) printf("\t\t");
                    printf("|\t\t%s %s = ", getDataTypeName(tmp->dataType), tmp->name);

                    // hodnota
                    tVariableData * data = tmp->data;
                    if (data != NULL) {
                        if (data->data != NULL) {
                            int * valI;
                            double * valD;
                            char * valS;
                            switch (tmp->dataType) {
                                case INTEGER:
                                    valI = (int *) data->data;
                                    printf("%d", *valI);
                                    break;
                                case DOUBLE:
                                    valD = (double *) data->data;
                                    printf("%f", *valD);
                                    break;
                                case STRING:
                                    valS = (char *) data->data;
                                    printf("%s", valS);
                                    break;
                                default:
                                    printf(" ||UNSPEC|| ");
                                    break;
                            }
                        }
                        else {
                            printf("NULL");
                        }
                    }
                    else {
                        printf("NULL");
                    }

                    printf("\n");
                }

                tmp = tmp->nextItem;
            }

        }
        else { // no synonyms
            if (item->type == function) {
                // function
                if (addPadding) printf("\t\t");
                printf("|\t%s %s(", getDataTypeName(item->dataType), item->name);

                //parametry
                tFunctionData * fData = (tFunctionData *) item->data;
                tFunctionParams *param1 =fData->params;
                while (param1 != NULL) {
                    show_ht_item(param1->item, addPadding);
                    param1 = param1->nextParam;
                }

                printf(")");
                if (fData->defined) {
                    printf(" DEF");
                } else {
                    printf(" NOT DEF");
                }
                printf("\n");

                show_ht_funcTable(item);
            }
            else if (item->type == variable){
                // variable
                if (addPadding) printf("\t\t");
                printf("|\t%s %s = ", getDataTypeName(item->dataType), item->name);

                // hodnota
                tVariableData * data = item->data;
                if (data != NULL) {
                    if (data->data != NULL) {
                        int * valI;
                        double * valD;
                        char * valS;
                        switch (item->dataType) {
                            case INTEGER:
                                valI = (int *) data->data;
                                printf("%d", *valI);
                                break;
                            case DOUBLE:
                                valD = (double *) data->data;
                                printf("%f", *valD);
                                break;
                            case STRING:
                                valS = (char *) data->data;
                                printf("%s", valS);
                                break;
                            default:
                                printf(" ||UNSPEC|| ");
                                break;
                        }
                    }
                    else {
                        printf("NULL");
                    }
                }
                else {
                    printf("NULL");
                }


                printf("\n");
            }
            else if (item->type == parametr) {
                // parametr
                if (addPadding) printf("\t\t");
                printf("%s %s", getDataTypeName(item->dataType), item->name);
            }
        }
    }
}

const char *getDataTypeName(tokenTypes dataType) {

    switch (dataType) {
        case INTEGER:
            return "int";
        case DOUBLE:
            return "double";
        case STRING:
            return "string";
        default:
            break;
    }

    return "UNDEFINED";
}

void show_ht_funcTable(tItem *item) {

    if (item != NULL) {
        if (item->type == function) {
            ht_table * table = (ht_table*) &((tFunctionData*) (item->data))->sTable;

            if (table != NULL) {
                show_ht_table(table, true);
            }
        }
    }
}

void show_st_list(tFunctionParams *list) {

    if (list == NULL) {
        printf("NOT INIT");
    }

    // Je uz item
    tFunctionParams * tmp = list;
    // az na konec
    while (tmp != NULL) {

        if (tmp->item != NULL) {
            if (tmp->item->type == parametr) {
                printf("%s %s", getDataTypeName(tmp->item->dataType), tmp->item->name);
            }
        }

        tmp = tmp->nextParam;
    }
}
