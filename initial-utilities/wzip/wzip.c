// #include <stdio.h>
// #include "../../common.h"
// #include "zipentry.h"
// #include "../../project-lib/mem/mem.h"
// #include <string.h>
// #include <stdlib.h>

// #define MAX_ENTRY 8192 // 256 kinds of characters such as alphabet and special chracters
// #define MAX_LINE 8192
// #define MAX_BATCH 8192000000

// Zip_T entries[MAX_ENTRY] = {NULL}; // Initialize all entries to NULL
// size_t entryIndex = 0;


// char *readMultipleFiles(char **files, int numFiles, long *totalSizeOut) {
//     long totalSize = 0;

//     // first pass: calculate total size of all files
//     for (size_t i = 1; i < numFiles; i++) {
//         FILE *fp = fopen(files[i], "rb");
//         if (!fp) {
//             fprintf(stderr, "Cannot open file %s\n", files[i]);
//             return NULL;
//         }
//         fseek(fp, 0, SEEK_END);
//         long size = ftell(fp);
//         fclose(fp);

//         totalSize += size;
//     }


//     char *buffer = ALLOC(totalSize + 1);
//     if (!buffer) {
//         fprintf(stderr, "Memory alloacation failed\n");
//         return NULL;
//     }

//     // second pass: Read files into the buffer
//     long pos = 0;
//     for (size_t i = 1; i < numFiles; i++) {
//         FILE *fp = fopen(files[i], "rb");
//         if (!fp) {
//             fprintf(stderr, "Cannot open file %s\n", files[i]);
//             return NULL;
//         }

//         fseek(fp, 0, SEEK_END);
//         long size = ftell(fp);
//         rewind(fp);

//         size_t readBytes = fread(buffer + pos, 1, size, fp);
//         if (readBytes != size) {
//             fprintf(stderr, "Reading error %s\n", files[i]);
//             fclose(fp);
//             FREE(buffer);
//             return NULL;
//         }
//         fclose(fp);
//         pos += size;
//     }

//     buffer[totalSize] = '\0';

//     if (totalSizeOut)
//         *totalSizeOut = totalSize;
//     return buffer;

// }


// int zipping(char *batch, const char *filename) {
//     FILE *fp;

    
//     fp = fopen(filename, "rb");

//     if (fp == NULL) {
//         fprintf(stderr, "Cannot open file\n");
//         return 1;
//     }

//     fseek(fp, 0, SEEK_END);
//     long fileSize = ftell(fp);
//     rewind(fp);

//     batch = ALLOC(fileSize + 1);

//     if (batch == NULL) {
//         printf("Memory error.\n");
//         fclose(fp);
//         return 1;
//     }

//     fread(batch, 1, fileSize, fp);
//     batch[fileSize] = '\0';

//     // ssize_t nread;
//     // size_t len;
//     // while ((nread = getline(&buffer, &len, fp)) != -1) {
//     //     strncat(batch, buffer, nread);
//     // }

//     // FREE(batch); // must free buffer even if the getline() failed
//     fclose(fp);
//     return 0;
// }

// void freeArr(Zip_T arr[]) {
//     size_t index = 0;

//     while (arr[index] != NULL) {
//         FREE(arr[index]);
//         index++;
//     }
// }

// int main(int argc, char *argv[]) {

//     char *programName = (char *)malloc(sizeof(argv[0]));
//     GetPrgName(argv[0], programName);

//     if (argc < 2) {
//         printf("%s: file1 [file2 ...]\n", programName);
//         return 1;
//     } else {
        
//         // the shell already handled redirecting output stream by using ">"
//         // We don't need to handle all tokens from user's command line
//         // We only need to handle all arguments which precede the ">"
//         // char *batch = ALLOC(MAX_BATCH * sizeof(char));
//         // for (size_t i = 1; i < argc; i++) {
//         //     zipping(batch, argv[i]);
//         // }

//         long totalSize = 0;

//         char *batch = readMultipleFiles(argv, argc, &totalSize);

//         if (batch != NULL) {

//             printf("%s", batch);
//             size_t batchIdx = 0;

//             char curChar = *(batch + batchIdx);
//             Zip_T entry = Zip_new(curChar);
//             batchIdx++;
//             char nextChar = *(batch + batchIdx);

//             if (nextChar == '\0'){
//                 entries[entryIndex] = entry;
//             } else {
//                 while (nextChar != '\0') {
//                     if (nextChar != curChar) {
//                         entries[entryIndex++] = entry; // Add current entry to the array
//                         entry = Zip_new(nextChar);  // initialize new entry for the next character
//                     } else {
//                         entry->count += 1;  // update the current entry
//                     }
//                     // shift forward next char and cur char
//                     curChar = nextChar;
//                     batchIdx++;
//                     nextChar = *(batch + batchIdx);
//                     // If the next character is the terminated character then add the current entry to the array
//                     if (nextChar == '\0') {
//                         entries[entryIndex] = entry;
//                     }
//                 }
//             }
//             FREE(batch);
//         } else {
//             fprintf(stderr, "Reading files fail!\n");
//             return 1;
//         }
//     }

        

//     size_t index = 0;
//     while (entries[index] != NULL) {
//         fwrite(entries[index], 5, 1, stdout);
//         // printf("%d%c", entries[index]->count, entries[index]->character);
//         index++;
//     }

//     FREE(programName);
//     freeArr(entries);

//     return 0;
// }

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        // fprintf(stderr, "wzip: file1 [file2 ...]\n");
        printf("wzip: file1 [file2 ...]\n");
        exit(1);
    }

    int count = 0;
    char curr, prev;
    int first = 1; // flag to track the first character

    for (int i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        if (fp == NULL) {
            fprintf(stderr, "wzip: cannot open file\n");
            exit(1);
        }

        // Read one character at a time
        while (fread(&curr, sizeof(char), 1, fp) == 1) {
            if (first) {
                prev = curr;
                count = 1;
                first = 0;
            } else if (curr == prev) {
                count++;
            } else {
                // Output previous run: [count][character]
                fwrite(&count, sizeof(int), 1, stdout);
                fwrite(&prev, sizeof(char), 1, stdout);
                prev = curr;
                count = 1;
            }
        }

        fclose(fp);
    }

    // Write last run if file not empty
    if (!first) {
        fwrite(&count, sizeof(int), 1, stdout);
        fwrite(&prev, sizeof(char), 1, stdout);
    }

    return 0;
}
