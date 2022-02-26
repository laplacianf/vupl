#ifndef __EXECUTE_H__
#define __EXECUTE_H__

typedef union Object {
    OperationList* body;
} Object;


typedef struct Runnable {
    OperationList* body;
    struct TValue* variables;
} Runnable;


typedef union Value {
    int i;
    Object* o;
    Runnable* r;
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

TValue execute(Runnable* runnable, int type);
void executeMain(const char* code);

#endif