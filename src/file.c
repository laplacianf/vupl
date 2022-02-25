#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"

#define addChar(s, c)\
    s = realloc((s), (strlen((s)) + 2)*sizeof(char));\
    strcat(s, (char[]){(c), 0})\

char* open(const char* file) {
    FILE* fp = fopen(file, "r");

    char* code = calloc(1, sizeof(char));
    char current;

    if (fp != NULL) {
        while ((current = fgetc(fp)) != EOF) {
            addChar(code, current);
        }
    } 
    else {
        //file do not exist
    }

    fclose(fp);

    return code;
}