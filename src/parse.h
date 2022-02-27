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
11 CREATETEMPLATE
12 CREATEOBJ → ;
13 GETATTR → .
14 SETATTR → ,
15 PRINT → ?
16 PRINTC → ~
17 ENDFILE 
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
    CREATETEMPLATE,
    CREATEOBJ,
    GETATTR,
    SETATTR,
    PRINT,
    PRINTC,
    ENDFILE
} OperationType;


typedef union OperationInfo {
    int pos; //stores information about jump position
    struct OperationList* body; //stores information about body
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