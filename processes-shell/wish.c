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

char *strCpy(char *dst, const char *src) {
    char *s = dst;

    while (*dst++ = *src++)
        ;
        
    return s;
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

void formatLine(const char *line, char buf[]) {
    int index = 0;

    while (isspace(*line)) {
        ++line;
    }

    int i = 0;
    while (*line != '\n') {
        buf[i] = *line;
        i++;
        ++line;
    }
    buf[i] = '\0';
}

int main(int argc, char *argv[]) {

    const char *program = argv[0];

    char *programName = getProgramName(program);

    if (argc > 2) {
        fprintf(stderr, "%s or %s <string>\n", programName, programName);
        exit(1);
    }

    
    

    if (programName != NULL) {
        char *line = NULL;
        size_t len = 0;
        while (1) {
            printf("%s> ", programName);
            if ((getline(&line, &len, stdin)) != -1) {

                // remove spaces and format the input line
                char formatedLine[MAX_LINE];
                formatLine(line, formatedLine);

                if (strCmp(formatedLine, "exit") == 0) {
                    printf("exit!!!\n");
                    break;
                }
            }
            
            
        }
        free(line);
        free(programName);
    }
    

    return 0;
}