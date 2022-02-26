#ifndef __EXECUTE_H__
#define __EXECUTE_H__

typedef union Template {
    OperationList* body;
} Template;


typedef struct Runnable {
    OperationList* body;
    struct TValue* variables;
} Runnable;


typedef union Value {
    int i;
    Template* t;
    Runnable* r;
} Value;


typedef enum ValueType {
    INT = 0,
    TEMPLATE,
    RUNNABLE 
} ValueType;


typedef struct TValue { //value with types
    Value value;
    ValueType type;
} TValue;


typedef struct Stack { 
    TValue* list;
    int size;
    int last;
} Stack;

TValue execute(Runnable* runnable, int type);
void executeMain(const char* code);

#endif