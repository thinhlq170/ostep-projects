#include "common.h"

void GetPrgName(const char *programArg, char *programName) {
    
    if (programName == NULL || sizeof(programName) < (sizeof(programArg) - 2)) {
        printf("programeName was not allocated sufficiently!\n");
        return;
    }

    size_t i = 2;
    while (*(programArg + i) != '\0') {
        *(programName + i - 2) = *(programArg + i);
        i++;
    }

    *(programName + i - 2) = '\0';
}