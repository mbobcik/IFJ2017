//
// Created by Martin Bobčík on 10/23/17.
//
#include "scanner.h"


token * getNextToken() {

    // Pokud je nejaky token na znovuzpracovani
    if (haveSomeTokenToProcess()) {
        return getTokenToProcess();
    }

    token *newToken;
    if (!(newToken = (token *) malloc(sizeof(token)))) {
        throwError(INTERNAL_ERROR, __LINE__);
    }
    if (!(newToken->data = malloc(sizeof(char) * TOKEN_DATA_SIZE))) {
        throwError(INTERNAL_ERROR, __LINE__);
    }

    FAStates state = startFlag;

    buffer *buffer = bInit(2);
    int escapeSeqNumber=0;
    char c;

    while ((c = (char)fgetc(input))) {

         if (state == identifierFlag) {
            if (isalpha(c) || isdigit(c) || c == '_') {   // identifier alnum_ >> identifier
                bAdd(tolower(c), buffer);
            } else {                                      // identifier else >> identifier end
                // is Keyword?
                newToken->tokenType = idOrKey(buffer->data);
                strcpy(newToken->data, buffer->data);
                ungetc(c, input);
                return newToken;
            }
        } else if (state == integerFlag) {
            if (isdigit(c)) {                       // integer 0-9 >> integer
                bAdd(c, buffer);
            } else if (c == '.') {                  // integer '.' >> dotFlag
                state = dotFlag;
                bAdd(c, buffer);
            } else if (c == 'e' ||
                   c == 'E') {                      // integer eE >> EFlag
                state = EFlag;
                bAdd(tolower(c), buffer);

            } else if (isalpha(c) || c == '_' || c == '$') {
                throwError(LEXICAL_ERROR, __LINE__);
            } else {                                // integer else >> integer end
                newToken->tokenType = INTEGER;
                strcpy(newToken->data, buffer->data);
                ungetc(c, input);

                if (INT_MAX == ((int) strtol(newToken->data, (char **) NULL, 10)))      // FIXME
                    throwError(LEXICAL_ERROR, __LINE__);

                return newToken;
            }
        } else if (state == dotFlag) {
            if (isdigit(c)) {                       // dotFlag 0-9 >> double
                bAdd(c, buffer);
                state = doubleFlag;
            } else {
                throwError(LEXICAL_ERROR, __LINE__);
            }
        } else if (state == doubleFlag) {
            if (isdigit(c)) {                       // double 0-9 >> double
                bAdd(c, buffer);
            } else if (c == 'e' ||                  // double eE  >> EFlag
                       c == 'E') {
                state = EFlag;
                bAdd(tolower(c), buffer);
            } else if (isalpha(c) || c == '_' || c == '$') {
                throwError(LEXICAL_ERROR, __LINE__);
            } else {                                // double else >> double end
                newToken->tokenType = DOUBLE;
                strcpy(newToken->data, buffer->data);
                ungetc(c, input);

                if (FLT_MAX == ((double) strtod(newToken->data, NULL))) {   // FIXME
                    throwError(LEXICAL_ERROR, __LINE__);
                }

                return newToken;
            }
        } else if (state == EFlag) {
            if (isdigit(c)) {                   // EFlag 0-9 >> doubleE
                bAdd(c, buffer);
                state = doubleEFlag;
            } else if (c == '+' ||
                       c == '-') {              //EFlag +- >> ESign
                bAdd(c, buffer);
                state = ESignFlag;
            } else {
                throwError(LEXICAL_ERROR, __LINE__);
            }
        } else if (state == ESignFlag) {         // ESign 0-9 >> doubleE
            if (isdigit(c)) {
                bAdd(c, buffer);
                state = doubleEFlag;
            } else {
                throwError(LEXICAL_ERROR, __LINE__);
            }
        } else if (state == doubleEFlag) {
            if (isdigit(c)) {                     // doubleE 0-9 >> doubleE
                bAdd(c, buffer);
            } else if (isalpha(c) || c == '_' || c == '$') {
                throwError(LEXICAL_ERROR, __LINE__);
            } else {                              // double E else >> doubleE end
                newToken->tokenType = DOUBLE;
                strcpy(newToken->data, buffer->data);
                ungetc(c, input);

                if (FLT_MAX == ((double) strtod(newToken->data, NULL))) {       //FIXME
                    throwError(LEXICAL_ERROR, __LINE__);
                }

                return newToken;
            }
        } else if (state == lineCommentFlag) {              // line comment '\n' >> start
            if (c == '\n') {
                state = startFlag;
            }                                               // line comment else >> line comment
             if(c == EOF){
                 newToken->tokenType=END_OF_FILE;
                 return newToken;
             }
        } else if (state == possibleBlockCommentFlag) {
            if (c == APOSTROPHE_ASCII_VALUE) {              // possible block comment apostrophe >> block comment
                state = blockCommentFlag;
            } else {                                       // possible block comment else >> divide operator
                newToken->tokenType = OPERATOR_DIVIDE;
                ungetc(c, input);
                return newToken;
            }
        } else if (state == blockCommentFlag) {
            if (c == APOSTROPHE_ASCII_VALUE) {              // block comment apostrophe >> possible block uncomment
                state = possibleBlockUncommentFlag;
            }                                               // block comment else >> block comment
             if(c == EOF){
                 newToken->tokenType=END_OF_FILE;
                 return newToken;
             }
        } else if (state == possibleBlockUncommentFlag) {
            if (c == '/') {                                  // possible block uncomment '/' >> start
                state = startFlag;
            } else {                                         // possible block uncomment else >> block comment
                state = blockCommentFlag;
            }
             if(c == EOF){
                 newToken->tokenType=END_OF_FILE;
                 return newToken;
             }
        } else if (state == assignFlag) {
            if (c == '=') {                                 // assign '=' >> Equal operator
                newToken->tokenType = OPERATOR_EQUAL;
                return newToken;
            } else {
                newToken->tokenType = OPERATOR_ASSIGN;      // assign else >> assign operator
                ungetc(c, input);
                return newToken;
            }
        } else if (state == lesserFlag) {
            if (c == '=') {                                 // lesser '=' >> lesser or equal
                newToken->tokenType = OPERATOR_LESSER_EQUAL;
                return newToken;
            } else if (c == '>') {
                newToken->tokenType = OPERATOR_NOT_EQUAL;   // lesser '>' >> not equal  (<>)
                return newToken;
            } else {                                         // lesser else >> lesser
                newToken->tokenType = OPERATOR_LESSER;
                ungetc(c, input);
                return newToken;
            }
        } else if (state == greaterFlag) {
            if (c == '=') {                                 // greater '=' >> greater or equal
                newToken->tokenType = OPERATOR_GREATER_EQUAL;
                return newToken;
            } else {                                        // greater else >> greater
                newToken->tokenType = OPERATOR_GREATER;
                ungetc(c, input);
                return newToken;
            }
        } else if (state == toBeStringFlag) {
            if (c == '"') {                                   // toBeString '"' >> string
                state = stringFlag;
            }
        } else if (state == stringFlag) {                     // string '\" >> escape
            if (c == '\\') {
                state = escapeFlag;
            } else if (c > 31 && c != '\\' && c != '"') {     // string else >> string
                bAdd(c, buffer);
            } else if (c == '"') {                            // string '"' >> end string
                newToken->tokenType = STRING;
                strcpy(newToken->data, buffer->data);
                return newToken;
            } else {
                throwError(LEXICAL_ERROR, __LINE__);
            }
        }else if (state == escapeFlag) {
            if (c == '\\') {                                  // escape '\' >> string
                bAdd('\\', buffer);
                state = stringFlag;
            } else if (c == 'n') {                            // escape 'n' >> string
                bAdd('\n', buffer);
                state = stringFlag;
            } else if (c == 't') {                            // escape 't' >> string
                bAdd('\t', buffer);
                state = stringFlag;
            } else if (c == '"') {                            // escape '"' >> string
                bAdd('\"', buffer);
                state = stringFlag;
            } else if (isdigit(c)) {                           // escape 0-9 >> escape 1st number
                escapeSeqNumber = CHARNUMBER_TO_INT(c) * 100;
                state = escape1stNumFlag;
            } else {
                throwError(LEXICAL_ERROR, __LINE__);
            }
        } else if (state == escape1stNumFlag) {
            if (isdigit(c)) {                                  // escape 1st number 0-9 >> escape 2nd number
                escapeSeqNumber += CHARNUMBER_TO_INT(c) * 10;
                state = escape2ndNumFlag;
            } else {
                throwError(LEXICAL_ERROR, __LINE__);
            }
        } else if (state == escape2ndNumFlag) {
            if (isdigit(c)){                                   // escape 2nd number 0-9 >> string
                escapeSeqNumber += CHARNUMBER_TO_INT(c);
                if( escapeSeqNumber >255 || escapeSeqNumber < 1){
                    throwError(LEXICAL_ERROR,__LINE__);
                }
                bAdd(escapeSeqNumber,buffer);
                state = stringFlag;
            } else{
                throwError(LEXICAL_ERROR,__LINE__);
            }
        } else if (state == startFlag) {
             if (c == EOF) {              // start EOF >> EOF end
                 newToken->tokenType = END_OF_FILE;
                 newToken->data = "EOF";
                 bDispose(buffer);
                 return newToken;
             } else if (isalpha(c) ||
                c == '_') {             // start _a-zA-Z >> identifier
                state = identifierFlag;
                bAdd(tolower(c), buffer);
            } else if (isdigit(c)) {     // start number >> int
                state = integerFlag;
                bAdd(c, buffer);
            } else if (c == APOSTROPHE_ASCII_VALUE) { // start '\39' >> line_comment
                state = lineCommentFlag;
            } else if (c == '/') {      // start '/'>> possible_block_comment
                state = possibleBlockCommentFlag;
            } else if (isspace(c)) {     //start whiteSpace >> start
                state = startFlag;

                 if(c == '\n'){         // start '\n' >> EOL end
                     newToken->tokenType = END_OF_LINE;
                     return newToken;
                 }
            } else if (c == '=') {       // start '=' >> Assign
                state = assignFlag;
            } else if (c == '<') {       // start '<' >> Lesser
                state = lesserFlag;
            } else if (c == '>') {       // start '>' >> Greater
                state = greaterFlag;
            } else if (c == '!') {       // start '!' >> toBeString
                state = toBeStringFlag;
            } else if (c == '+') {       // start '+' >> plus operator end
                newToken->tokenType = OPERATOR_PLUS;
                return newToken;
            } else if (c == '-') {       // start '-' >> minus operator end
                newToken->tokenType = OPERATOR_MINUS;
                return newToken;
            } else if (c == '*') {       // start '*' >> multiply operator end
                newToken->tokenType = OPERATOR_MULTIPLY;
                return newToken;
            } else if (c == ','){        // start ',' >> comma end
                newToken->tokenType = COMMA;
                return newToken;
            } else if (c == '('){       // start '(' >> opening bracket end
                newToken->tokenType = OPENING_BRACKET;
                return newToken;
            } else if (c == ')'){        //start ')' >> closing bracket end
                newToken->tokenType = CLOSING_BRACKET;
                return newToken;
            } else if ( c == ';'){      // start ';' >> semicolon end
                newToken->tokenType = SEMICOLON;
                return newToken;
            } else {
                throwError(LEXICAL_ERROR,__LINE__);
            }
        }
    }
    throwError(INTERNAL_ERROR,__LINE__);
    return newToken;
}

tokenTypes idOrKey(char * data) {
    if (strcmp(data, "as") == 0) {
        return KEY_AS;
    } else if (strcmp(data, "asc") == 0) {
        return KEY_ASC;
    } else if (strcmp(data, "declare") == 0) {
        return KEY_DECLARE;
    } else if (strcmp(data, "dim") == 0) {
        return KEY_DIM;
    } else if (strcmp(data, "do") == 0) {
        return KEY_DO;
    } else if (strcmp(data, "double") == 0) {
        return KEY_DOUBLE;
    } else if (strcmp(data, "else") == 0) {
        return KEY_ELSE;
    } else if (strcmp(data, "end") == 0) {
        return KEY_END;
    } else if (strcmp(data, "chr") == 0) {
        return KEY_CHR;
    } else if (strcmp(data, "function") == 0) {
        return KEY_FUNCTION;
    } else if (strcmp(data, "if") == 0) {
        return KEY_IF;
    } else if (strcmp(data, "input") == 0) {
        return KEY_INPUT;
    } else if (strcmp(data, "integer") == 0) {
        return KEY_INTEGER;
    } else if (strcmp(data, "length") == 0) {
        return KEY_LENGTH;
    } else if (strcmp(data, "loop") == 0) {
        return KEY_LOOP;
    } else if (strcmp(data, "print") == 0) {
        return KEY_PRINT;
    } else if (strcmp(data, "return") == 0) {
        return KEY_RETURN;
    } else if (strcmp(data, "scope") == 0) {
        return KEY_SCOPE;
    } else if (strcmp(data, "string") == 0) {
        return KEY_STRING;
    } else if (strcmp(data, "substr") == 0) {
        return KEY_SUBSTR;
    } else if (strcmp(data, "then") == 0) {
        return KEY_THEN;
    } else if (strcmp(data, "while") == 0) {
        return KEY_WHILE;
    } else if (strcmp(data, "and") == 0) {
        return KEY_AND;
    } else if (strcmp(data, "boolean") == 0) {
        return KEY_BOOLEAN;
    } else if (strcmp(data, "continue") == 0) {
        return KEY_CONTINUE;
    } else if (strcmp(data, "elseif") == 0) {
        return KEY_ELSEIF;
    } else if (strcmp(data, "exit") == 0) {
        return KEY_EXIT;
    } else if (strcmp(data, "false") == 0) {
        return KEY_FALSE;
    } else if (strcmp(data, "for") == 0) {
        return KEY_FOR;
    } else if (strcmp(data, "next") == 0) {
        return KEY_NEXT;
    } else if (strcmp(data, "not") == 0) {
        return KEY_NOT;
    } else if (strcmp(data, "or") == 0) {
        return KEY_OR;
    } else if (strcmp(data, "shared") == 0) {
        return KEY_SHARED;
    } else if (strcmp(data, "static") == 0) {
        return KEY_STATIC;
    } else if (strcmp(data, "true") == 0) {
        return KEY_TRUE;
    } else{
        return IDENTIFIER;
    }
}

buffer * bInit(int size){
    buffer * newBuffer;
    if(!(newBuffer = (buffer *)malloc(sizeof(buffer)))) {
        throwError(INTERNAL_ERROR, __LINE__);
    }
    newBuffer->actualSize = 0;
    newBuffer->alocatedSize = size;
    if(!(newBuffer->data = malloc(size * sizeof(char)))){
        throwError(INTERNAL_ERROR,__LINE__);
    }
    newBuffer->data[0]='\0';
    return newBuffer;
}

void bAdd(char c, buffer * buffer){
    if(!buffer){
        throwError(INTERNAL_ERROR,__LINE__);
    }

    if(buffer->actualSize + 1 > buffer->alocatedSize){

        buffer->alocatedSize = 2 * buffer->alocatedSize;

        buffer->data = realloc(buffer->data, buffer->alocatedSize * sizeof(char));

        if(!buffer->data){
            throwError(INTERNAL_ERROR,__LINE__);
        }
    }

    buffer->data[buffer->actualSize++] = c;
    buffer->data[buffer->actualSize] = '\0';
}

void bDispose(buffer * buffer){
    if(!buffer){
        throwError(INTERNAL_ERROR,__LINE__);
    }
    free(buffer->data);
    free(buffer);
}
void printBuffer(buffer * buffer){
    if(buffer){
        printf("Buffer->allocatedSize: %d\nBuffer->actualSize: %d\nBuffer->data: %s\n",buffer->alocatedSize,buffer->actualSize,buffer->data);
    }
}

void printToken(token * token){
    printf("token->tokenType: %2d; token->data: %s\n",token->tokenType,token->data);
}


/*
 * Ulozi posledni token znovu ke zpracovani
 *
 * @author Meluzin
 */
void putTokenBack(token * token) {
    oldTokenToProcess = token;
}

/*
 * Zkontroluje jestli je nejaky token ke
 * znovu zpracovani
 *
 * @author Meluzin
 */
int haveSomeTokenToProcess(){
    return (oldTokenToProcess == NULL)? 0: 1;
}

/*
 * Vrati token ke znovuzpracovani
 *
 * @author Meluzin
 */
token * getTokenToProcess() {
    if (haveSomeTokenToProcess()) {
        token * ret = oldTokenToProcess;
        oldTokenToProcess = NULL;

        return ret;
    }
    return NULL;
}

token *getToken() {
    return nextToken;
}
