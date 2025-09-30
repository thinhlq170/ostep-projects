#include "wish.h"

#define MAX_LINE 1024

char *getProgramName(const char *program) {
    size_t programeNameLength = strlen(program);

    char *programName = (char *)malloc(programeNameLength * sizeof(char));

    if (program != NULL) {
        int i;
        for (i = 2; i < programeNameLength; i++) {
            *(programName + i - 2) = *(program + i);
        }
        *(programName + i) = '\0';
        return programName;
    }

    return NULL;
}

int strCmp(const char *s1, const char *s2) {
    int i = 0;
    size_t s1len = strlen(s1);
    size_t s2len = strlen(s2);

    if (s1len > s2len) {
        return *(s1 + s2len) + 0;
    } else if (s1len < s2len) {
        return 0 - *(s2 + s1len);
    } else {
        while (*(s1 + i) != '\0' && *(s2 + i) != '\0') {
            if (*(s1 + i) == *(s2 + i)) {
                i++;
            } else {
                return *(s1 + i) - *(s2 + i);
            }
            
        }

    }
    
    return 0;
}

void formatLine(const char *curLine, char buf[]) {
    const char *line = curLine;

    // skip leading spaces
    while (isspace(*line)) {
        ++line;
    }

    int i = 0;
    while (*line != '\n') {
        // skip redundant spaces between words in the line
        if (isspace(*line)) {
            int j = 1;
            while (isspace(*(line + j))) { // *(line + j) is the next character
                line++;
            }
        }
        buf[i] = *line;
        i++;
        ++line;
    }
    // replace '\n' by '\0' to mark end of line
    buf[i] = '\0';
}

void handleInteractiveMode(char *argv[]) {
    const char *program = argv[0];

    char *programName = getProgramName(program);

    if (programName != NULL) {
        char *line = NULL;
        size_t len = 0;
        while (1) {
            printf("%s> ", programName);
            if ((getline(&line, &len, stdin)) != -1) {

                // remove spaces and format the input line
                char formatedLine[MAX_LINE];
                formatLine(line, formatedLine);

                char *originalLine = strdup(formatedLine);
                char *curLine = originalLine;

                char *command;
                if ((command = strsep(&curLine, " ")) != NULL) {
                    if (strCmp(command, EXIT_COMMAND) == 0) {
                        break;
                    } else if (strCmp(command, CHDIR_COMMAND) == 0) {
                        printf("built-in command: %s\n", command);
                    } else if (strCmp(command, PATH_COMMAND) == 0) {
                        printf("built-in command: %s\n", command);
                    } else {
                        printf("user command: %s\n", command);
                    }
                }

                char *firstArg;
                if ((firstArg = strsep(&curLine, " ")) != NULL) {
                    printf("The first argument: %s\n", firstArg);
                }

                free(originalLine);
            }  
        }
        free(line);
        free(programName);
    }
}



int main(int argc, char *argv[]) {

    const char *program = argv[0];

    char *programName = getProgramName(program);

    if (argc > 2) {
        fprintf(stderr, "%s or %s <string>\n", programName, programName);
        exit(1);
    } else if (argc == 2) {
        // handle batch mode
    } else {
        handleInteractiveMode(argv);
    }

    
    free(programName);

    return 0;
}