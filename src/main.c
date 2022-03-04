#include <stdio.h>
#include <stdlib.h>

#include "parse.h"
#include "execute.h"
#include "file.h"

int main(int argc, char* argv[]) {
    if (argc == 2) {
        char* code = open(argv[1]);
        executeCode(code);
    }
    else {
        printf("\nUsage: \'[file name]\'\n");
        exit(1);
    }
    
    return 0;
}