#ifndef MAIN_SYMTABLE_H
#define MAIN_SYMTABLE_H

#include "scanner.h" // pro typy tokenu
#include <stdbool.h>

/*
 * Maximalni velikost tabulky
 */
#define MAX_HTTABLE_SIZE 101

/*
 * Identifikator symbolu
 */
typedef char * tName;

typedef enum {
    variable,
    function,
    parametr
} tItemType;



/*
 * Polozka tabulky s odkazem na synonyma
 */
typedef struct tItem {
    tName           name;       // Nazev polozky (klic)
    tokenTypes      dataType;   // Datovy typ
    void *          data;       // Data (NULL - pro parametr funkce| tFunctionData | tVariableData)
    tItemType       type;       // Typ zaznamu (funkce | variable)
    struct tItem*   nextItem;   // Ukazatel na dalsi synonymum
} tItem;



typedef tItem* ht_table[MAX_HTTABLE_SIZE];

/*
 * Data pro promennou
 */
typedef struct tVariableData {
    void *          data;       // ukazatel na hodnotu
} tVariableData;

/*
 * Data pro funkci
 */
// Seznam parametru
typedef struct tFunctionParams {
    struct tItem* item;                 // aktualni Item (bude obsahovat jen name a dataType)
    struct tFunctionParams* firstParam; // prvni parametr
    struct tFunctionParams* nextParam;  // dalsi parametr
} tFunctionParams;

typedef struct tFunctionData {
    struct tFunctionParams* params;     // parametry funkce
    ht_table                sTable;     // tabulka symbolu pro funkci
    bool                    defined;    // definovana
} tFunctionData;


#include "functions.h"


/*
 * Rozptylovací funkce. Zdroj: Martin
 *
 * @author Meluzin
 * @param name
 */
int ht_hashCode(tName name);

void ht_init(ht_table* prtTable);

tItem* ht_search(ht_table* prtTable, tName name);

tItem* ht_insert(ht_table* prtTable, tName name);

tItem* ht_read(ht_table* prtTable, tName name);

void ht_delete(ht_table* prtTable, tName name);

void ht_deleteItem(tItem * item);

void ht_clearAll(ht_table* prtTable);

void ht_deleteParam(tFunctionParams * param);



///
/// Seznam
///

/*
 * Vytvoreni seznamu pro parametry
 *
 * @author Meluzin
 *
 * @param list
 */
void st_init(tFunctionParams * list);

/*
 * Pridani do seznamu
 *
 * @author Meluzin
 *
 * @param list
 * @param item
 * @return bool
 */
bool st_add(tFunctionParams * list, tItem * item);

/*
 * Odstraneni posledniho z listu
 *
 * @author Meluzin
 *
 * @param list
 */
void st_removeLast(tFunctionParams * list);

/*
 * Odstraneni vsech z listu
 *
 * @author Meluzin
 *
 * @param list
 */
void st_clear(tFunctionParams * list);




///
/// Rozhrani
///


/*
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

/*
 * Nastavi hodnotu promenne
 *
 * @author Meluzin
 *
 * @param ptrTable  tabulka symbolu
 * @param name      jmeno promenne
 * @param data      data do promenne (predpoklada ze byla malloc)
 * @return int
 *              0 neni tabulka
 *              -1 neexistujici promenna
 *              -2 je to funkce
 *              -3 interni erro (malloc...)
 *              1 ok
 */
int ht_setVarValue(ht_table * ptrTable, char * name, void * data);

/*
 * Vrati hodnotu promenne
 *
 * @author Meluzin
 *
 * @param ptrTable  tabulka symbolu
 * @param name      jmeno promenne
 * @return void *   ukazatel na data (null pokud neexistuje, nebo je to funkce)
 */
void * ht_getVarValue(ht_table * ptrTable, char * name);

/*
 * Kontrola existence promenne v tabulce symbolu
 *
 * @author Meluzin
 *
 * @param ptrTable  tabulka symbolu
 * @param name      jmeno promenne
 * @return int
 *              1 exist
 *              0 not exist
 *              -1 exist function
 *              -2 non existing table
 */
int ht_isVarExist(ht_table * ptrTable, char * name);

/*
 * Kontrola jestli ma promenna v tab i definovanou hodnotu
 *
 * @author Meluzin
 *
 * @param item
 * @return bool
 */
bool ht_isVarDefinedItem(tItem * item);

/*
 * Kontrola jestli ma promenna v tab i definovanou hodnotu
 *
 * @author Meluzin
 *
 * @param ptrTable  tabulka symbolu
 * @param name      jmeno promenne
 * @return bool
 */
bool ht_isVarDefined(ht_table * ptrTable, char * name);


/*
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

/*
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

/*
 * Vrati tabulku symbolu pro danou funkci
 *
 * @author Meluzin
 *
 * @param ptrTable  tabulka symbolu
 * @param functionName
 * @return ht_table *  NULL pro neexistujici
 */
ht_table * ht_getTableFor(ht_table * ptrTable, char * functionName);


/*
 * Zkontroluje jestli je funkce definovana
 *
 * @author Meluzin
 *
 * @param funkce
 * @return bool
 */
bool ht_isFuncDefinedItem(tItem * funkce);


/*
 * Zkontroluje jestli je funkce definovana
 *
 * @author Meluzin
 *
 * @param ptrTable  tabulka symbolu
 * @param functionName
 * @return bool
 */
bool ht_isFuncDefined(ht_table * ptrTable, char * functionName);

/*
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


/*
 * Nastavi funkci priznak ze je definovana
 *
 * @author Meluzin
 *
 * @param funkce
 */
void ht_setFuncDefinedItem(tItem * funkce);

/*
 * Nastavi funkci priznak ze je definovana
 *
 * @author Meluzin
 *
 * @param ptrTable  tabulka symbolu
 * @param functionName
 */
void ht_setFuncDefined(ht_table * ptrTable, char * functionName);


/*
 * Zkontroluje jestli je vse definovano
 *
 * @author Meluzin
 *
 * @return bool
 */
bool ht_isAllDefined(ht_table * ptrTable);



/*
 * Vytvoreni jednoho itemu pro parametry
 *
 * @author Meluzin
 * @param name
 * @param dataType
 * @return tItem *
 */
tItem * ht_creatItem(tName name, tokenTypes dataType);

#endif //MAIN_SYMTABLE_H
