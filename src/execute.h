#ifndef __EXECUTE_H__
#define __EXECUTE_H__


typedef union Object {
    OperationList* body;
} Object;


typedef union Value {
    int i;
    Object o;
} Value;


typedef enum ValueType {
    INT = 0,
    OBJECT,
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

void execute(OperationList* oplist);

#endif