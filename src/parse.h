#ifndef __PARSE_H__
#define __PARSE_H__

/*
00 PUSH → :
01 LOAD → <
02 STORE → >
03 ADD → +
04 SUB → -
05 IFZERO → [
06 LOOP → ]
07 BEGIN → {
08 END → }
09 RETURN → !
10 CALL → |
11 CREATEOBJ → ,
12 ATTR → .
13 PRINT → ?
14 PRINTC → ~
15 ENDFILE 
*/
typedef enum OperationType {
    PUSH = 0, 
    LOAD, 
    STORE, 
    ADD,
    SUB,
    IFZERO,
    LOOP,
    BEGIN,
    END,
    RETURN,
    CALL,
    CREATEOBJ,
    ATTR,
    PRINT,
    PRINTC,
    ENDFILE
} OperationType;


typedef union OperationInfo {
    int pos; //stores information about jump position
    struct OperationList* body; //stores information about objects body
} OperationInfo;


typedef struct Operation {
    OperationType type;
    OperationInfo info; //additional information about the operation
} Operation;


typedef struct OperationList {
    Operation* list;
    int size;
    int last;
} OperationList;


typedef struct PosStack { //stores position to check []
    int* list;
    int size;
    int last;
} PosStack;


typedef struct Parser {
    const char* code;
    int pos;
    char current;
} Parser;

OperationList* parse(const char* code);

#endif