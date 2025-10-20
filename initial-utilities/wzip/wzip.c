#include <stdio.h>
#include "../../common.h"

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
    }

    free(programName);

    return 0;
}