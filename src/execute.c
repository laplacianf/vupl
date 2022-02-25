#include <stdlib.h>
#include <stdio.h>

#include "parse.h"
#include "execute.h"

static TValue zero() {
    Value value;
    value.i = 0;

    TValue tvalue = { value, INT };

    return tvalue;
}

static int toInt(TValue tvalue) {
    if (tvalue.type == INT) {
        return tvalue.value.i;
    } 
    else {
        return 0;
    }
}

static void resizeStack(Stack* stack) {
    stack->list = realloc(stack->list, (stack->size * 2) * sizeof(TValue));
    stack->size *= 2;
}

static void push(Stack* stack, TValue tvalue) {
    if (stack->last >= stack->size - 1) {
        resizeStack(stack);
    }

    stack->list[++stack->last] = tvalue;
}

static TValue pop(Stack* stack) {
    if (stack->last >= 0) {
        TValue value = stack->list[stack->last];
        
        --stack->last;
                        
        return value;
    }
    else {
        return zero(); //return 0 when stack is empty
    }
}

static TValue add(TValue tvalue) {
    if (tvalue.type == INT) {
        int value = tvalue.value.i;

        Value newValue;
        newValue.i = value + 1;

        TValue newTvalue = { newValue, INT };
        return newTvalue;
    }
    else {
        return zero(); //return 0 when value is not int
    }
}

static TValue sub(TValue tvalue) {
    if (tvalue.type == INT) {
        int value = tvalue.value.i;

        Value newValue;
        newValue.i = value - 1;

        TValue newTvalue = { newValue, INT };
        return newTvalue;
    }
    else {
        return zero(); //return 0 when value is not int
    }
}

void execute(OperationList* oplist) {
    Stack* stack = malloc(sizeof(Stack));
    //initiallize stack
    stack->list = malloc(sizeof(TValue));
    stack->size = 1;
    stack->last = -1;

    TValue variables[256] = { zero() }; 

    int pos = 0;

    for (;;) {
        Operation op = oplist->list[pos];

        if (op.type == PUSH) {
            push(stack, zero());
        }
        else if (op.type == LOAD) {
            int var = toInt(pop(stack));
            
            if (var >= 0 && var <= 255) {
                push(stack, variables[var]);
            } 
            else {
                //raise error
            }
        }
        else if (op.type == STORE) {
            int var = toInt(pop(stack));

            if (var >= 0 && var <= 255) {
                variables[var] = pop(stack);
            }
            else {
                //raise error
            }
        }
        else if (op.type == ADD) {
            TValue val = pop(stack);
            push(stack, add(val));
        }
        else if (op.type == SUB) {
            TValue val = pop(stack);
            push(stack, sub(val));
        }
        else if (op.type == IFZERO) {
            int jump = op.info.pos;
            int val = toInt(pop(stack));

            if (val == 0) {
                pos = jump;
            } 
        }
        else if (op.type == LOOP) {
            int loop = op.info.pos;

            pos = loop - 1;
        }
        else if (op.type == CREATEOBJ) {
            Object obj = { op.info.body };
            Value value;
            value.o = obj;
            TValue tvalue = { value, OBJECT };

            push(stack, tvalue);
        }
        else if (op.type == PRINT) {
            int val = toInt(pop(stack));
            printf("%d", val);
        }
        else if (op.type == PRINTC) {
            int val = toInt(pop(stack));
            printf("%c", val);
        }
        else if (op.type == ENDFILE) {
            break;
        }

        ++pos;
    }

    printf("\n");

    for (int j = 0; j <= 32; j++) {
        if (variables[j].type == OBJECT) {
            printf("object\n");
        }
        else {
            printf("%d| %d\n", j, variables[j].value.i);
        }
    }
    
    free(stack);
    free(oplist);
}
