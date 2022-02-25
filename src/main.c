#include <stdio.h>

#include "parse.h"
#include "execute.h"
#include "file.h"

int main() {
    char* code = open("test.txt");

    OperationList* parseResult = parse(code);

    for (int i = 0; i <= parseResult->last; i++) {
        printf("%d| %d\n", i, parseResult->list[i].type);
    }

    execute(parseResult);

    return 0;
}