#include <stdlib.h>
#include <stdio.h>

#include "parse.h"
#include "execute.h"

#define VARIABLE_NUM 256

#define EXECUTE_FUNCTION 0
#define EXECUTE_OBJECT 1

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

static void initVariables(Runnable* runnable) {
    for (int i = 0; i < VARIABLE_NUM; i++) {
        runnable->variables[i] = zero();
    }
}

static void initParam(Runnable* runnable, Stack* stack) {
    for (int i = 0; i <= stack->last + 1; i++) {
        runnable->variables[i + 1] = pop(stack);
    }
}

static TValue execute(Runnable* runnable, int type) {
    OperationList* oplist = runnable->body;

    Stack* stack = malloc(sizeof(Stack));
    //initiallize stack
    stack->list = malloc(sizeof(TValue));
    stack->size = 1;
    stack->last = -1;

    int pos = 0;

    for (;;) {
        Operation op = oplist->list[pos];

        if (op.type == PUSH) {
            push(stack, zero());
        }
        else if (op.type == LOAD) {
            int var = toInt(pop(stack));
            
            if (var >= 0 && var < VARIABLE_NUM) {
                push(stack, runnable->variables[var]);
            } 
            else {
                //raise error
            }
        }
        else if (op.type == STORE) {
            int var = toInt(pop(stack));

            if (var >= 0 && var < VARIABLE_NUM) {
                runnable->variables[var] = pop(stack);
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
        else if (op.type == CREATETEMPLATE) {
            Template* newTemplate = malloc(sizeof(Template));
            //initiallize new template
            newTemplate->body = malloc(sizeof(OperationList));
            newTemplate->body = op.info.body;
            Value value;
            value.t = newTemplate;
            TValue tvalue = { value, TEMPLATE };

            push(stack, tvalue);
        }
        else if (op.type == RETURN) {
            if (type == EXECUTE_FUNCTION) {
                break;
            }
            else {
                printf("\nReturn Outside Function\n");
                exit(1);
            }

        }
        else if (op.type == CALL) {
            TValue template = pop(stack);
            if (template.type == TEMPLATE) {
                Runnable* newRunnable = malloc(sizeof(Runnable));
                //initiallize new runnable
                newRunnable->variables = malloc(sizeof(TValue) * 256);
                newRunnable->body = malloc(sizeof(OperationList));

                newRunnable->body = template.value.t->body;

                initVariables(newRunnable);

                newRunnable->variables[0] = runnable->variables[0]; //set variable 0 to previous scope's variable 0
                
                initParam(newRunnable, stack);

                push(stack, execute(newRunnable, EXECUTE_FUNCTION));
                
                free(newRunnable);
            }
            else {
                //raise error
            }
        }
        else if (op.type == CREATEOBJ) {
            TValue template = pop(stack);
            if (template.type == TEMPLATE) {
                Runnable* newRunnable = malloc(sizeof(Runnable));
                //initiallize new runnable
                newRunnable->variables = malloc(sizeof(TValue) * 256);
                newRunnable->body = malloc(sizeof(OperationList));

                newRunnable->body = template.value.t->body;

                Value value;
                value.r = runnable;
                TValue tvalue = { value, RUNNABLE };

                initVariables(newRunnable);

                newRunnable->variables[0] = tvalue; //set variable 0 of new runnable to this
                
                initParam(newRunnable, stack);

                execute(newRunnable, EXECUTE_OBJECT);

                Value newValue;
                newValue.r = newRunnable;
                TValue newTvalue = { newValue, RUNNABLE };
                
                push(stack, newTvalue);
            } 
            else {
                //raise error
            }
        }
        else if (op.type == GETATTR) {
            TValue object = pop(stack);
            int attr = toInt(pop(stack));

            if (object.type == RUNNABLE) {
                if (attr >= 0 && attr < VARIABLE_NUM) {
                    push(stack, object.value.r->variables[attr]);
                }
                else {
                    //raise error
                }
            }
            else {
                //raise error
            }
        }
        else if (op.type == SETATTR) {
            TValue object = pop(stack);
            int attr = toInt(pop(stack));

            if (object.type == RUNNABLE) {
                if (attr >= 0 && attr < VARIABLE_NUM) {
                    object.value.r->variables[attr] = pop(stack);
                }
                else {
                    //raise error
                }
            }
            else {
                //raise error
            }
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
    
    TValue ret = pop(stack);
    
    free(stack);

    return ret;
}

void executeCode(const char* code) {
    OperationList* parseResult = parse(code);

    Runnable* main = malloc(sizeof(Runnable));
    //initiallize main runnable
    main->variables = malloc(sizeof(TValue) * 256);
    main->body = malloc(sizeof(OperationList));

    main->body = parseResult;

    initVariables(main);

    Value value;
    value.r = main;

    TValue tvalue = { value, RUNNABLE };

    main->variables[0] = tvalue; //set variable 0 to this

    execute(main, EXECUTE_OBJECT);

    free(main);
} 