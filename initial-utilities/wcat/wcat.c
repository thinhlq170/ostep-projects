#include <stdio.h>
#include <stdlib.h>
#include "../../common.h"

#define MAX_LINE 1024

void proceed_wcat(const char *programName, char *filePath) {
    FILE *file = fopen(filePath, "r");

    char buffer[MAX_LINE];
    
    
    if (file != NULL) {
        while (fgets(buffer, MAX_LINE, file) != NULL) {
            printf("%s", buffer);
        }
    } else {
        printf("%s: cannot open file\n", programName);
        exit(1);
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    
    char *programName = (char *)malloc(sizeof(argv[0]));
    GetPrgName(argv[0], programName);

    // exclude the first argument which is the program name
    for (int i = 1; i < argc; i++) {
        proceed_wcat(programName, argv[i]);
    }

    free(programName);

    return 0;
}