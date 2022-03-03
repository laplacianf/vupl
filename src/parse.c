#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "parse.h"

static void advance(Parser* parser) {
    ++parser->pos;
    parser->current = parser->code[parser->pos];
}

static void resizeOpList(OperationList* oplist) {
    oplist->list = realloc(oplist->list, (oplist->size * 2) * sizeof(Operation));
    oplist->size *= 2;
}

static void addOperation(OperationList* oplist, Operation op) {
    if (oplist->last >= oplist->size - 1) {
        resizeOpList(oplist);
    }

    oplist->list[++oplist->last] = op;
}

static void resizePosStack(PosStack* ps) {
    ps->list = realloc(ps->list, (ps->size * 2) * sizeof(int));
    ps->size *= 2;
}

static void pushPos(PosStack* ps, int value) {
    if (ps->last >= ps->size - 1) {
        resizePosStack(ps);
    }

    ps->list[++ps->last] = value;
}

static int popPos(PosStack* ps) {
    if (ps->last >= 0) {
        int value = ps->list[ps->last];
        
        --ps->last;
                        
        return value;
    }
    else {
        return -1;
    }
}

static OperationList* matchLoop(OperationList* oplist) {
    PosStack* posStack = malloc(sizeof(PosStack));
    //initiallize stack
    posStack->list = malloc(sizeof(int));
    posStack->size = 1;
    posStack->last = -1;

    for (int i = 0; i <= oplist->last; i++) {
        if (oplist->list[i].type == IFZERO) {
            pushPos(posStack, i);
        }
        else if (oplist->list[i].type == LOOP) {
            int pos = popPos(posStack);

            if (pos >= 0) {
                OperationInfo loopInfo;
                loopInfo.pos = pos;
                oplist->list[i].info = loopInfo;

                OperationInfo ifzeroInfo;
                ifzeroInfo.pos = i;
                oplist->list[pos].info = ifzeroInfo;
            }
            else {
                printf("\nInvalid Syntax \']\'\n");
                exit(1);
            }
        }
    }

    if (posStack->last >= 0) {
        printf("\nUnclosed \'[\'\n");
        exit(1);
    }
    
    free(posStack);

    return oplist;
}

OperationList* parse(const char* code) {
    Parser* parser = malloc(sizeof(Parser));
    //initiallize parser
    parser->code = code;
    parser->pos = 0;
    parser->current = code[0];

    OperationList* result = malloc(sizeof(OperationList));
    //initiallize result
    result->list = malloc(sizeof(Operation));
    result->size = 1;
    result->last = -1;

    int opened = 0;

    for (;;) {
        if (parser->pos > strlen(parser->code) - 1) break;

        if (parser->current == ':') {
            Operation op = { PUSH };
            addOperation(result, op);
        }
        else if (parser->current == '<') {
            Operation op = { LOAD };
            addOperation(result, op);
        }
        else if (parser->current == '>') {
            Operation op = { STORE };
            addOperation(result, op);
        }
        else if (parser->current == '+') {
            Operation op = { ADD };
            addOperation(result, op);
        }
        else if (parser->current == '-') {
            Operation op = { SUB };
            addOperation(result, op);
        }
        else if (parser->current == '[') {
            Operation op = { IFZERO };
            addOperation(result, op);
        }
        else if (parser->current == ']') {
            Operation op = { LOOP };
            addOperation(result, op);
        }
        else if (parser->current == '{') {
            opened = 1;
            char* body = calloc(1, sizeof(char));

            advance(parser);

            for (;;) {
                if (parser->current == '{') ++opened;
                if (parser->current == '}') --opened;

                if (opened == 0) break;

                if (parser->pos > strlen(parser->code) - 1) {
                    printf("\nUnclosed \'{\'\n");
                    exit(1);
                }

                body = realloc(body, (strlen(body) + 2) * sizeof(char));
                strcat(body, (char[]){parser->current, 0});

                advance(parser);
            }

            OperationInfo templateInfo;
            templateInfo.body = parse(body);

            Operation op = { CREATETEMPLATE, templateInfo };
            addOperation(result, op);

            free(body);
        }
        else if (parser->current == '!') {
            Operation op = { RETURN };
            addOperation(result, op);
        }
        else if (parser->current == '|') {
            Operation op = { CALL };
            addOperation(result, op);
        }
        else if (parser->current == ';') {
            Operation op = { CREATEOBJ };
            addOperation(result, op);
        }
        else if (parser->current == '.') {
            Operation op = { GETATTR };
            addOperation(result, op);
        }
        else if (parser->current == ',') {
            Operation op = { SETATTR };
            addOperation(result, op);
        }
        else if (parser->current == '?') {
            Operation op = { PRINT };
            addOperation(result, op);
        }
        else if (parser->current == '~') {
            Operation op = { PRINTC };
            addOperation(result, op);
        }
        else {
            if (parser->current == '\n' || parser->current == '\t' || parser->current == ' ') {
            }
            else if (parser->current == '#') { //comment
                while (parser->current != '\n') {
                    advance(parser);
                }
            }
            else {
                printf("\nInvalid Syntax \'%c\'\n", parser->current);
                exit(1);
            }
        }

        advance(parser);
    }

    matchLoop(result);
    Operation op = { ENDFILE };
    addOperation(result, op);

    free(parser);
    return result;
}