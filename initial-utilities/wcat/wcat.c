#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE 1024

void proceed_wcat(char *filePath) {
    FILE *file = fopen(filePath, "r");

    char buffer[MAX_LINE];
    
    if (file != NULL) {
        while (fgets(buffer, MAX_LINE, file) != NULL) {
            printf("%s", buffer);
        }
    } else {
        printf("Cannot open file %s\n", filePath);
        exit(0);
    }
}

int main(int argc, char *argv[]) {

    for (int i = 1; i < argc; i++) {
        proceed_wcat(argv[i]);
    }

    return 0;
}