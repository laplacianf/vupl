#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"

char* open(const char* file) {
    FILE* fp = fopen(file, "r");

    char* code = calloc(1, sizeof(char));
    char current;

    if (fp != NULL) {
        while ((current = fgetc(fp)) != EOF) {
            code = realloc(code, (strlen(code) + 2)*sizeof(char));
            strcat(code, (char[]){current, 0});
        }
    } 
    else {
        //file do not exist
    }

    fclose(fp);

    return code;
}