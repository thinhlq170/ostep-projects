#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE 1024

void proceed_wcat(const char *program_name, char *filePath) {
    FILE *file = fopen(filePath, "r");

    char buffer[MAX_LINE];
    
    
    if (file != NULL) {
        while (fgets(buffer, MAX_LINE, file) != NULL) {
            printf("%s", buffer);
        }
    } else {
        printf("%s: cannot open file\n", program_name);
        exit(1);
    }

    fclose(file);
}

int main(int argc, char *argv[]) {

    // take the program name
    char *program = argv[0];
    char *program_name = (char *)malloc(sizeof(argv[0]));
    int i = 2; // exlcude 2 first characters ./
    while (*(program + i) != '\0') {
        *(program_name + i - 2) = *(program + i);
        i++;
    }

    *(program_name + i - 2) = '\0';

    // exclude the first argument which is the program name
    for (int i = 1; i < argc; i++) {
        proceed_wcat(program_name, argv[i]);
    }

    free(program_name);

    return 0;
}