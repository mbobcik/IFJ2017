//
// Created by root on 12/5/17.
//

#ifndef SYMTABLE_TYPES_H
#define SYMTABLE_TYPES_H



FILE *input;

typedef struct buffer{
    int alocatedSize;
    int actualSize;
    char * data;
}buffer;

typedef enum FAStates {
    startFlag,
    identifierFlag,
    integerFlag,
    dotFlag,
    EFlag,
    lineCommentFlag,
    possibleBlockCommentFlag,
    blockCommentFlag,
    possibleBlockUncommentFlag,
    assignFlag,
    greaterFlag,
    lesserFlag,
    toBeStringFlag,
    stringFlag,
    escapeFlag,
    escape1stNumFlag,
    escape2ndNumFlag,
    doubleFlag,
    doubleEFlag,
    ESignFlag
} FAStates;

typedef enum tokenTypes {
    IDENTIFIER, // 0
    INTEGER,
    DOUBLE,
    STRING,
    KEY_AS,
    KEY_ASC,
    KEY_DECLARE,
    KEY_DIM,
    KEY_DO,
    KEY_DOUBLE,
    KEY_ELSE, // 10
    KEY_END,
    KEY_CHR,
    KEY_FUNCTION,
    KEY_IF,
    KEY_INPUT,
    KEY_INTEGER,
    KEY_LENGTH,
    KEY_LOOP,
    KEY_PRINT,
    KEY_RETURN, // 20
    KEY_SCOPE,
    KEY_STRING,
    KEY_SUBSTR,
    KEY_THEN,
    KEY_WHILE,
    END_OF_FILE,
    OPERATOR_PLUS,
    OPERATOR_MINUS,
    OPERATOR_MULTIPLY,
    OPERATOR_DIVIDE, // 30
    OPERATOR_GREATER,
    OPERATOR_GREATER_EQUAL,
    OPERATOR_LESSER,
    OPERATOR_LESSER_EQUAL,
    OPERATOR_EQUAL,
    OPERATOR_NOT_EQUAL,
    OPERATOR_ASSIGN,
    COMMA,
    OPENING_BRACKET,
    CLOSING_BRACKET, // 40
    SEMICOLON,
    END_OF_LINE,

    /////////Reserved keywords
            KEY_AND = 1001,
    KEY_BOOLEAN = 1002,
    KEY_CONTINUE = 1003,
    KEY_ELSEIF = 1004,
    KEY_EXIT = 1005,
    KEY_FALSE = 1006,
    KEY_FOR = 1007,
    KEY_NEXT = 1008,
    KEY_NOT = 1009,
    KEY_OR = 1010,
    KEY_SHARED = 1011,
    KEY_STATIC = 1012,
    KEY_TRUE = 1013
} tokenTypes;

typedef struct token{
    tokenTypes tokenType;
    char * data;
}token;


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
    struct tFunctionParams* nextParam;  // dalsi parametr
} tFunctionParams;

typedef struct tFunctionData {
    struct tFunctionParams* params;     // parametry funkce
    ht_table                sTable;     // tabulka symbolu pro funkci
    bool                    defined;    // definovana
} tFunctionData;


token * oldTokenToProcess;
token * nextToken;
ht_table globalSymTable;

// Tabulka symbolu pro aktualni kontext
ht_table * contextSymTable;

char *actuallyParsedFunction;

//
typedef struct tTokensList {
    token * token;
    struct tTokensList * first;
    struct tTokensList * next;
} tTokensList;

typedef enum {
    noType_t,
    string_t,
    integer_t,
    double_t,
} tExpresionType;

#endif //SYMTABLE_TYPES_H
