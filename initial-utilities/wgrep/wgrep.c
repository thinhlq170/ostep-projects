#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../common.h"

int isContained(const char *searchterm, const char *line) {
    for (size_t l = 0; l < strlen(line); l++) {
        if (*(line + l) == *(searchterm)) {
            int matched = 0;
            l++;
            for (size_t s = 1; s < strlen(searchterm); s++, l++) {
                if (*(searchterm + s) != *(line + l)) {
                    matched = -1;
                    break;
                }
            }
            if (matched == 0)
                return matched;
        }
    }
    return -1;
}

void grepping(const char *searchterm, FILE *stream) {
    char *buffer;
    size_t n;

    while ((getline(&buffer, &n, stream)) != -1) {
        if (isContained(searchterm, buffer) == 0) {
            printf("%s", buffer);
        }
    }
    // manpage: This buffer should be freed by the user program even if getline() failed
    free(buffer);
}


int main(int argc, char *argv[]) {

    char *programName = (char *)malloc(sizeof(argv[0]));
    
    GetPrgName(argv[0], programName);


    if (argc < 2) { 
        printf("%s: searchterm [file ...]\n", programName);
        return 1;

    } else if (argc == 2) { // search in stdin
        grepping(argv[1], stdin);
    } else { // search in files
        int i = 2;
        
        while (argv[i] != NULL) {
            FILE *file;
            file = fopen(argv[i], "r");

            if (file != NULL) {
                grepping(argv[1], file);
            } else {
                printf("%s: cannot open file\n", programName);
                return 1;
            }

            fclose(file);
            i++;
        }
    }

    free(programName);

    return 0;
}