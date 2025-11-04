#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("wunzip: file1 [file2 ...]\n");
        return 1;
    }

    for (size_t i = 1; i < argc; i++) {
        char *fileName = argv[i];
        FILE *fp = fopen(fileName, "rb");

        if (fp == NULL) {
            printf("Cannot open file %s\n", fileName);
            return 1;
        }

        int numRead;
        char charRead;
        
        // uncompress each chunk of data which was compromised by 4 bytes of integer and 1 byte of char
        while (fread(&numRead, 4, 1, fp) == 1) {
            if (fread(&charRead, 1, 1, fp) == 1) {
                for (int i = 0; i < numRead; i++) {
                    printf("%c", charRead);
                }
            }
        }
        fclose(fp);
    }

    return 0;
}