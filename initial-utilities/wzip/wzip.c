#include <stdio.h>
#include "../../common.h"
#include "zipentry.h"
#include "../../project-lib/mem/mem.h"

#define MAX_ENTRY 256 // 256 kinds of characters such as alphabet and special chracters



Zip_T *entries[MAX_ENTRY];

void zipping(const char *filename) {
    
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
        printf("argc: %d\n", argc);

        Zip_T zip = Zip_new('a');
        printf("character %c exists %d times\n", zip->character, zip->count);
        FREE(zip);
    }

    free(programName);

    return 0;
}