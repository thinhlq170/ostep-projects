#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void getProgramName(const char *programArg, char *programName) {
    
    if (sizeof(programName) < (sizeof(programArg) - 2)) {
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

void greping(const char *searchterm, FILE *stream) {
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
    
    getProgramName(argv[0], programName);


    if (argc < 2) { 
        printf("%s: searchterm [file ...]\n", programName);
        return 1;

    } else if (argc == 2) { // search in stdin
        greping(argv[1], stdin);
    } else { // search in files
        int i = 2;
        
        while (argv[i] != NULL) {
            FILE *file;
            file = fopen(argv[i], "r");

            if (file != NULL) {
                greping(argv[1], file);
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