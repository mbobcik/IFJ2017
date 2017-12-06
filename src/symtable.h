#ifndef MAIN_SYMTABLE_H
#define MAIN_SYMTABLE_H



#include "functions.h"

#define HT_FUNC_OK              1 // ok func
#define HT_FUNC_BAD_PARAMS      0 // no same params
#define HT_FUNC_BAD_DATATYPE    -1 // not same datatype
#define HT_FUNC_BAD_NAME        -2 // not same name
#define HT_FUNC_NOT_FUNC        -3 // not a func
#define HT_FUNC_NOT_FOUND       -4 // not found
#define HT_NULL_TABLE           -5 // null tabulka
#define HT_VAR_NOT_FOUD         -4 // promenna nenalezena
#define HT_VAR_NOT_VAR          -10 // not variable
#define HT_VAR_OK               2   // ok var
#define HT_VAR_BAD_DATATYPE     -11 // neshodny datatyp
#define HT_ITERNAL_ERROR        -69 // interni error


int ht_hashCode(tName name);

void ht_init(ht_table* prtTable);

tItem* ht_search(ht_table* prtTable, tName name);

tItem* ht_insert(ht_table* prtTable, tName name);

void ht_delete(ht_table* prtTable, tName name);

void ht_deleteItem(tItem * item);

void ht_clearAll(ht_table* prtTable);



///
/// Seznam
///

/**
 * Vytvoreni seznamu pro parametry
 *
 * @author Meluzin
 *
 * @param list
 */
void st_init(tFunctionParams ** list);

/**
 * Pridani do seznamu
 *
 * @author Meluzin
 *
 * @param list
 * @param item
 * @return bool false - seznam neni inicializovan | jiz existuje
 *              true - pridano
 */
bool st_add(tFunctionParams * list, tItem * item);

/**
 * Odstraneni posledniho z listu
 *
 * @author Meluzin
 *
 * @param list
 */
void st_removeLast(tFunctionParams ** list);

/**
 * Odstraneni vsech z listu
 *
 * @author Meluzin
 *
 * @param list
 */
void st_clear(tFunctionParams * list);

/**
 * Porovna jestli obsahuje listA ty stejne prvky jako listB
 *
 * @author Meluzin
 *
 * @param listA
 * @psram listB
 * @return bool
 */
bool st_isSame(tFunctionParams * listA, tFunctionParams * listB);


///
/// Rozhrani
///


/**
 * Kontrola existence v listu
 *
 * @author Meluzin
 *
 * @param list
 * @param name
 * @return bool
 */
bool st_isExist(tFunctionParams * list, char * name);

/**
 * Pridani do listu jako parametr
 *
 * @author Meluzi
 *
 * @param list
 * @param name
 * @param dataType
 * @return bool
 */
bool st_addParam(tFunctionParams * list, char * name, tokenTypes dataType);

/**
 * Pridani variable do ht
 *
 * @author Meluzin
 *
 * @param ptrTable  tabulka symbolu
 * @param name      jmeno promenne
 * @param dataType  datovy typromenne
 * @return tItem *  NULL pokud jiz existuje
 */
tItem * ht_addVariable(ht_table * ptrTable, char * name, tokenTypes dataType);


/**
 * Nastavi hodnotu promenne. Data je potreba alokovat predem pomoci malloc
 *
 * @author Meluzin
 *
 * @param item
 * @param data      data do promenne (predpoklada ze byla malloc)
 * @return int
 *              HT_VAR_NOT_FOUD     neexistujici promenna
 *              HT_VAR_NOT_VAR      je to funkce
 *              HT_ITERNAL_ERROR    interni erro (malloc...)
 *              HT_VAR_OK           ok
 */
int ht_setVarValueItem(tItem * item, void * data);

/**
 * Nastavi hodnotu promenne. Data je potreba alokovat predem pomoci malloc
 *
 * @author Meluzin
 *
 * @param ptrTable  tabulka symbolu
 * @param name      jmeno promenne
 * @param data      data do promenne (predpoklada ze byla malloc)
 * @return int
 *              HT_NULL_TABLE       neni tabulka
 *              HT_VAR_NOT_FOUD     neexistujici promenna
 *              HT_VAR_NOT_VAR      je to funkce
 *              HT_ITERNAL_ERROR    interni erro (malloc...)
 *              HT_VAR_OK           ok
 */
int ht_setVarValue(ht_table * ptrTable, char * name, void * data);


/**
 * Nastavi hodnotu int promenne. Data si zkopiruje pomoci memcpy.
 *
 * @author Meluzin
 *
 * @param ptrTable  tabulka symbolu
 * @param name      jmeno promenne
 * @param data      data do promenne (zkopiruje si je pomoci mamcpy)
 * @return int
 *              HT_NULL_TABLE       neni tabulka
 *              HT_VAR_NOT_FOUD     neexistujici promenna
 *              HT_VAR_NOT_VAR      je to funkce
 *              HT_ITERNAL_ERROR    interni erro (malloc...)
 *              HT_VAR_BAD_DATATYPE pouzity spatny datatyp
 *              HT_VAR_OK           ok
 */
int ht_setVarValueInt(ht_table *ptrTable, char *name, int data);


/**
 * Nastavi hodnotu int promenne. Data si zkopiruje pomoci memcpy.
 *
 * @author Meluzin
 *
 * @param ptrTable  tabulka symbolu - globalni
 * @param funcName  jmeno tabulky
 * @param varName   jmeno promenne
 * @param data      data do promenne (zkopiruje si je pomoci mamcpy)
 * @return int
 *              HT_NULL_TABLE       neni tabulka
 *              HT_VAR_NOT_FOUD     neexistujici promenna
 *              HT_VAR_NOT_VAR      je to funkce
 *              HT_ITERNAL_ERROR    interni erro (malloc...)
 *              HT_VAR_BAD_DATATYPE pouzity spatny datatyp
 *              HT_VAR_OK           ok
 */
int ht_setVarValueIntForFunc(ht_table *ptrTable, char *funcName, char *varName, int data);

/**
 * Nastavi hodnotu int promenne. Data si zkopiruje pomoci memcpy.
 *
 * @author Meluzin
 *
 * @param ptrTable  tabulka symbolu
 * @param name      jmeno promenne
 * @param data      data do promenne (zkopiruje si je pomoci mamcpy)
 * @return int
 *              HT_NULL_TABLE       neni tabulka
 *              HT_VAR_NOT_FOUD     neexistujici promenna
 *              HT_VAR_NOT_VAR      je to funkce
 *              HT_ITERNAL_ERROR    interni erro (malloc...)
 *              HT_VAR_BAD_DATATYPE pouzity spatny datatyp
 *              HT_VAR_OK           ok
 */
int ht_setVarValueDouble(ht_table *ptrTable, char *name, double data);



/**
 * Nastavi hodnotu int promenne. Data si zkopiruje pomoci memcpy.
 *
 * @author Meluzin
 *
 * @param ptrTable  tabulka symbolu - globalni
 * @param funcName  jmeno tabulky
 * @param varName   jmeno promenne
 * @param data      data do promenne (zkopiruje si je pomoci mamcpy)
 * @return int
 *              HT_NULL_TABLE       neni tabulka
 *              HT_VAR_NOT_FOUD     neexistujici promenna
 *              HT_VAR_NOT_VAR      je to funkce
 *              HT_ITERNAL_ERROR    interni erro (malloc...)
 *              HT_VAR_BAD_DATATYPE pouzity spatny datatyp
 *              HT_VAR_OK           ok
 */
int ht_setVarValueDoubleForFunc(ht_table *ptrTable, char *funcName, char *varName, double data);

/**
 * Nastavi hodnotu int promenne. Data si zkopiruje pomoci memcpy.
 *
 * @author Meluzin
 *
 * @param ptrTable  tabulka symbolu
 * @param name      jmeno promenne
 * @param data      data do promenne (zkopiruje si je pomoci mamcpy)
 * @return int
 *              HT_NULL_TABLE       neni tabulka
 *              HT_VAR_NOT_FOUD     neexistujici promenna
 *              HT_VAR_NOT_VAR      je to funkce
 *              HT_ITERNAL_ERROR    interni erro (malloc...)
 *              HT_VAR_BAD_DATATYPE pouzity spatny datatyp
 *              HT_VAR_OK           ok
 */
int ht_setVarValueString(ht_table *ptrTable, char *name, char *data);


/**
 * Nastavi hodnotu int promenne. Data si zkopiruje pomoci memcpy.
 *
 * @author Meluzin
 *
 * @param ptrTable  tabulka symbolu - globalni
 * @param funcName  jmeno tabulky
 * @param varName   jmeno promenne
 * @param data      data do promenne (zkopiruje si je pomoci mamcpy)
 * @return int
 *              HT_NULL_TABLE       neni tabulka
 *              HT_VAR_NOT_FOUD     neexistujici promenna
 *              HT_VAR_NOT_VAR      je to funkce
 *              HT_ITERNAL_ERROR    interni erro (malloc...)
 *              HT_VAR_BAD_DATATYPE pouzity spatny datatyp
 *              HT_VAR_OK           ok
 */
int ht_setVarValueStringForFunc(ht_table *ptrTable, char *funcName, char *varName, char *data);

/**
 * Vrati hodnotu promenne
 *
 * @author Meluzin
 *
 * @param ptrTable  tabulka symbolu
 * @param name      jmeno promenne
 * @return void *   ukazatel na data (null pokud neexistuje, nebo je to funkce)
 */
void * ht_getVarValue(ht_table * ptrTable, char * name);

/**
 * Vrati hodnotu promenne jako Integer.
 * Pouze pokud je to Integer. Jinak NULL
 *
 * @author Meluzin
 *
 * @param item
 * @return int *
 */
int * ht_getVarValueIntItem(tItem * item);

/**
 * Vrati hodnotu promenne jako Integer.
 * Pouze pokud je to Integer. Jinak NULL
 *
 * @author Meluzin
 *
 * @param ptrTable  tabulka symbolu
 * @param name      jmeno promenne
 * @return int *
 */
int * ht_getVarValueInt(ht_table * ptrTable, char * name);


/**
 * Vrati hodnotu promenne jako double.
 * Pouze pokud je to double. Jinak NULL
 *
 * @author Meluzin
 *
 * @param item
 * @return double *
 */
double * ht_getVarValueDoubleItem(tItem * item);

/**
 * Vrati hodnotu promenne jako double.
 * Pouze pokud je to double. Jinak NULL
 *
 * @author Meluzin
 *
 * @param ptrTable  tabulka symbolu
 * @param name      jmeno promenne
 * @return double *
 */
double * ht_getVarValueDouble(ht_table * ptrTable, char * name);

/**
 * Vrati hodnotu promenne jako string.
 * Pouze pokud je to string. Jinak NULL
 *
 * @author Meluzin
 *
 * @param item
 * @return char *
 */
char * ht_getVarValueStringItem(tItem * item);

/**
 * Vrati hodnotu promenne jako string.
 * Pouze pokud je to string. Jinak NULL
 *
 * @author Meluzin
 *
 * @param ptrTable  tabulka symbolu
 * @param name      jmeno promenne
 * @return char *
 */
char * ht_getVarValueString(ht_table * ptrTable, char * name);


/**
 * Vrati datovy typ polozky pokud se ji podari najit
 *
 * @author Meluzin
 *
 * @param ptrTable  tabulka symbolu
 * @param name      jmeno polozky
 * @return tokenTypes
 */
tokenTypes ht_getDataType(ht_table * ptrTable, char * name);

/**
 * Vrati datovy typ Polozky. Jinak null
 *
 * @author Meluzin
 *
 * @param item
 * @return tokenTypes
 */
tokenTypes ht_getDataTypeItem(tItem * item);


/**
 * Kontrola existence promenne v tabulce symbolu
 *
 * @author Meluzin
 *
 * @param ptrTable  tabulka symbolu
 * @param name      jmeno promenne
 * @return int
 *              HT_VAR_OK           exist
 *              HT_VAR_NOT_FOUD     not exist
 *              HT_VAR_NOT_VAR      exist function
 *              HT_NULL_TABLE       non existing table
 */
int ht_isVarExist(ht_table * ptrTable, char * name);



/**
 * Kontrola jestli ma promenna v tab i definovanou hodnotu
 *
 * @author Meluzin
 *
 * @param item
 * @return bool
 */
bool ht_isVarDefinedItem(tItem * item);

/**
 * Kontrola jestli ma promenna v tab i definovanou hodnotu
 *
 * @author Meluzin
 *
 * @param ptrTable  tabulka symbolu
 * @param name      jmeno promenne
 * @return bool
 */
bool ht_isVarDefined(ht_table * ptrTable, char * name);


/**
 * Pridani funkci do ht
 *
 * @author Meluzin
 *
 * @param ptrTable  tabulka symbolu
 * @param name      jmeno funkce
 * @param dataType  datovy typ funkce
 * @return tItem *  NULL pokud jiz existuje
 */
tItem * ht_addFunction(ht_table * ptrTable, char * name, tokenTypes dataType);

/**
 * Pridani funkci do ht
 *
 * @author Meluzin
 *
 * @param ptrTable  tabulka symbolu
 * @param name      jmeno funkce
 * @param dataType  datovy typ funkce
 * @param params    parametry funkce
 * @return tItem *  NULL pokud jiz existuje
 */
tItem * ht_addFunctionWithParams(ht_table * ptrTable, char * name, tokenTypes dataType, tFunctionParams* params);

/**
 * Vrati tabulku symbolu pro danou funkci
 *
 * @author Meluzin
 *
 * @param ptrTable  tabulka symbolu
 * @param functionName
 * @return ht_table *  NULL pro neexistujici
 */
ht_table * ht_getTableFor(ht_table * ptrTable, char * functionName);


/**
 * Zkontroluje jestli je funkce definovana
 *
 * @author Meluzin
 *
 * @param funkce
 * @return bool
 */
bool ht_isFuncDefinedItem(tItem * funkce);


/**
 * Zkontroluje jestli je funkce definovana
 *
 * @author Meluzin
 *
 * @param ptrTable  tabulka symbolu
 * @param functionName
 * @return bool
 */
bool ht_isFuncDefined(ht_table * ptrTable, char * functionName);

/**
 * Zkontroluje jestli je funkce definovana
 * a zkontroluje jestli je definovana i tabulka funkce
 *
 * @author Meluzin
 *
 * @param ptrTable  tabulka symbolu
 * @param functionName
 * @return bool
 */
bool ht_isFuncDefinedWithTable(ht_table * ptrTable, char * functionName);


/**
 * Nastavi funkci priznak ze je definovana
 *
 * @author Meluzin
 *
 * @param funkce
 */
void ht_setFuncDefinedItem(tItem * funkce);

/**
 * Nastavi funkci priznak ze je definovana
 *
 * @author Meluzin
 *
 * @param ptrTable  tabulka symbolu
 * @param functionName
 */
void ht_setFuncDefined(ht_table * ptrTable, char * functionName);


/**
 * Zkontroluje jestli je vse definovano
 *
 * @author Meluzin
 *
 * @return bool
 */
bool ht_isAllDefined(ht_table * ptrTable);



/**
 * Vytvoreni jednoho itemu pro parametry
 *
 * @author Meluzin
 *
 * @param name
 * @param dataType
 * @return tItem *
 */
tItem * ht_creatItem(tName name, tokenTypes dataType);

/**
 * Nastaveni parametru pro danou funkci
 *
 * @author Meluzin
 *
 * @param item      selector funkce
 * @param params    list parametru
 * @return bool
 */
bool ht_setFuncParamsItem(tItem * item, tFunctionParams * params);

/**
 * Nastaveni parametru pro danou funkci
 * Selectorem funkce je tabulka symbolu a nazev funkce
 *
 * @author Meluzin
 *
 * @param ptrTable  tabulka symbolu
 * @param functionName
 * @param params    list parametru
 * @return bool
 */
bool ht_setFuncParams(ht_table * ptrTable, char * functionName, tFunctionParams * params);




/**
 * Je item funkci, kterou definuji parametry (name, params, returnType)
 *
 * @author Meluzin
 *
 * @param func      func k otestovani
 * @param name      jmeno funkce
 * @param retType   return type
 * @param params    parametry
 * @return int
 *                  HT_FUNC_NOT_FOUND       NULL func
 *                  HT_FUNC_NOT_FUNC        not func
 *                  HT_FUNC_BAD_NAME        different name
 *                  HT_FUNC_BAD_DATATYPE    different datatype
 *                  HT_FUNC_BAD_PARAMS      not same params
 *                  HT_FUNC_OK              OK all same
 */
int ht_isFunctionItem(tItem * func, char * name, tokenTypes retType, tFunctionParams * params);




/**
 * Je to funkce, kterou definuji parametry (name, params, returnType)
 *
 * @author Meluzin
 *
 * @param ptrTable  tabulka symbolu
 * @param name      jmeno funkce
 * @param retType   return type
 * @param params    parametry
 * @return int
 *                  HT_NULL_TABLE           null table
 *                  HT_FUNC_NOT_FOUND       not foud func
 *                  HT_FUNC_NOT_FUNC        not func
 *                  HT_FUNC_BAD_NAME        different name
 *                  HT_FUNC_BAD_DATATYPE    different datatype
 *                  HT_FUNC_BAD_PARAMS      not same params
 *                  HT_FUNC_OK              OK all same
 */
int ht_isFunction(ht_table * ptrTable, char * name, tokenTypes retType, tFunctionParams * params);


/**
 * Je to funkce?
 *
 * @author Meluzin
 *
 * @param ptrTable  tabulka symbolu
 * @param name      jmeno funkce
 * @param retType   return type
 * @param params    parametry
 * @return int
 *                  HT_NULL_TABLE           null table
 *                  HT_FUNC_NOT_FOUND       not foud func
 *                  HT_FUNC_NOT_FUNC        not func
 *                  HT_FUNC_OK              OK all same
 */
int ht_isFuncExist(ht_table * ptrTable, char * name);


/**
 * Ziskani promenne z dane funkce.
 * Pronde tabulku pro funkci a pak tabulku globalni
 *
 * @author Meluzin
 *
 * @param ptrTable  tabulka symbolu - globalni
 * @param funcName  jmeno funkce
 * @param varName   jmeno variable
 * @return tItem *  NULL pokud nenalezeno
 */
tItem * ht_getVarForFunc(ht_table * ptrTable, char * funcName, char * varName);


/**
 * Predela parametry na radky v tabulce symbolu
 *
 * @author Meluzin
 *
 * @param params
 * @param table
 * @return int 1 ok   -1 no params  -2  no table   -3 chyba pri pridavani do tabulky
 */
int ht_paramsToTable(tFunctionParams* params, ht_table * table);


#endif //MAIN_SYMTABLE_H
