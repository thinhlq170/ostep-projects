#include <stdio.h>
#include "../../common.h"
#include "zipentry.h"
#include "../../project-lib/mem/mem.h"
#include <string.h>
#include <stdlib.h>

#define MAX_ENTRY 256 // 256 kinds of characters such as alphabet and special chracters
#define MAX_LINE 256
#define MAX_BATCH 2048

Zip_T entries[MAX_ENTRY] = {NULL}; // Initialize all entries to NULL
size_t entryIndex = 0;


int zipping(char *batch, const char *filename) {
    FILE *fp;

    char *buffer = NULL;
    fp = fopen(filename, "r");

    if (fp == NULL) {
        fprintf(stderr, "Failed to open file\n");
        return 1;
    }
    ssize_t nread;
    size_t len;
    while ((nread = getline(&buffer, &len, fp)) != -1) {
        strncat(batch, buffer, nread);
    }

    FREE(buffer); // must free buffer even if the getline() failed
    fclose(fp);
    return 0;
}

void freeArr(Zip_T arr[]) {
    size_t index = 0;

    while (arr[index] != NULL) {
        FREE(arr[index]);
        index++;
    }
}

int main(int argc, char *argv[]) {

    char *programName = (char *)malloc(sizeof(argv[0]));
    GetPrgName(argv[0], programName);

    if (argc < 2) {
        printf("%s: file1 [file2 ...]\n", programName);
        return 1;
    } else {
        
        // the shell already handled redirecting output stream by using ">"
        // We don't need to handle all tokens from user's command line
        // We only need to handle all arguments which precede the ">"
        char *batch = ALLOC(MAX_BATCH * sizeof(char));
        for (size_t i = 1; i < argc; i++) {
            zipping(batch, argv[i]);
        }

        size_t batchIdx = 0;

        if (batch != NULL) {
            char curChar = *(batch + batchIdx);
            Zip_T entry = Zip_new(curChar);
            batchIdx++;
            char nextChar = *(batch + batchIdx);

            if (nextChar == '\0'){
                entries[entryIndex] = entry;
            } else {
                while (nextChar != '\0') {
                    if (nextChar != curChar) {
                        entries[entryIndex++] = entry; // Add current entry to the array
                        entry = Zip_new(nextChar);  // initialize new entry for the next character
                    } else {
                        entry->count += 1;  // update the current entry
                    }
                    // shift forward next char and cur char
                    curChar = nextChar;
                    batchIdx++;
                    nextChar = *(batch + batchIdx);
                    // If the next character is the terminated character then add the current entry to the array
                    if (nextChar == '\0') {
                        entries[entryIndex] = entry;
                    }
                }
            }
        }
        FREE(batch);
    }

    size_t index = 0;
    while (entries[index] != NULL) {
        fwrite(entries[index], 5, 1, stdout);
        // printf("%d%c", entries[index]->count, entries[index]->character);
        index++;
    }

    FREE(programName);
    freeArr(entries);

    return 0;
}