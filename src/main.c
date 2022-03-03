#include <stdio.h>

#include "parse.h"
#include "execute.h"
#include "file.h"

int main() {
    char* code = open("test.txt");

    executeCode(code);

    return 0;
}