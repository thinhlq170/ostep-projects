#include "wish.h"

#define MAX_LINE 1024
#define MAX_ARGS 128
#define MAX_WORD 64


void errorExit(int errNumber) {
    printf("An error has occurred\n");
    exit(errNumber);
}

void errThrow() {
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
}

char *getProgramName(const char *program) {
    // Exclude ./ from the program name. E.g ./wish -> wish
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
    // SINGLE-BYTE calculation
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
    char *orgLine = strdup(curLine);
    char *line = orgLine;
    

    // skip leading spaces
    while (isspace(*line)) {
        ++line;
    }

    int len = strlen(line);
     // HINTTT: THIS STRING IS TERMINATED BY '\n' not '\0'
    int k = len-1;
    while (k >= 0 && isspace(*(line + k))) {
        --k;
    }

    if (k < 0) {
        buf[0] = '\0';
    } else {
        // move the terminated marks after removing trailing spaces
        buf[k + 1] = '\0';
        *(line + k + 1) = '\n';
    }



    int i = 0;
    while (*line != '\n') {
        // skip redundant spaces between words in the line
        if (isspace(*line)) {
            int j = 1;
            while (isspace(*(line + j))) { // *(line + j) is the next character
                line++;
                j++;
            }
        }
        buf[i] = *line;
        i++;
        ++line;
    }
    // replace '\n' by '\0' to mark end of line
    buf[i] = '\0';
    free(orgLine);
}

char *getAccessedPath(char *command) {
    const char *dupCommand = strdup(command);
    char *binExePath = strdup(BIN_EXE_PATH);
    char *usrExePath = strdup(USR_EXE_PATH);

    binExePath = strcat(binExePath, dupCommand);
    usrExePath = strcat(usrExePath, dupCommand);

    int isBinExe = access(binExePath, X_OK);
    int isUsrExe = access(usrExePath, X_OK);

    if (isBinExe == -1 || isUsrExe == -1) {
        errThrow();
    }

    // TODO: handling case of input path is an absolute path

    if (isBinExe == 0) {
        return binExePath;
    } else if (isUsrExe == 0) {
        return usrExePath;
    }

    return NULL;

    
}

int isRedirectedCommand(char *args[], size_t argcSize) {
    if (argcSize > 1) {
        for (size_t i = argcSize - 1; i >= 0; i--) {
            if (strCmp(">", args[i]) == 0) {
                return 0;
            }
        }
    }
    
    return -1;
}

void freeCharArr(char *arr[]) {
    size_t i = 0;
    while (arr[i] != NULL) {
        free(arr[i++]);
    }
}

void handleUserCommand(char *originalLine) {
    char *curLine = originalLine;
    char *command;
    char *args[MAX_ARGS];

    
    if ((command = strsep(&curLine, " ")) != NULL) {
        args[0] = strdup(command);
        size_t i = 1;
        char *arg;
        while (curLine != NULL && (arg = strsep(&curLine, " ")) != NULL) {
            args[i] = strdup(arg);
            i++;
        }
        args[i] = NULL; // to mark end of arguments
        char *newEnv[] = { NULL };

        

        pid_t childPId;
        
        switch (childPId = fork())
        {
        case -1:
            errThrow();
            break;
        case 0:
            char *exePath = getAccessedPath(command);
            if (isRedirectedCommand(args, i) == 0) {
                // redirect standard output to the target file
                close(STDOUT_FILENO);
                open(args[i-1], O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
                char *commandArgs[MAX_ARGS];
                // take the command before the directed mark ">"
                size_t j = 0;
                // separated at ">" which is far from NULL 2 elements. But index starts with 0. Therefore, index subtract 3 instead of 2
                // TODO: handling more than 1 argument after ">"
                while (j <= i - 3) { 
                    commandArgs[j] = strdup(args[j]);
                    ++j;
                }
                commandArgs[j] = NULL;
                if ((execve(exePath, commandArgs, newEnv)) == -1) {
                    fprintf(stderr, "%s\n", strerror(errno));
                }

                freeCharArr(commandArgs);
            } else {
                if ((execve(exePath, args, newEnv)) == -1) {
                    errThrow();
                }
            }

            free(exePath);
                
            break;
        default:

            int commandStatus;

            waitpid(childPId, &commandStatus, 0);
            // if (WIFEXITED(commandStatus)) {
            //     int exitStatus = WEXITSTATUS(commandStatus);
            //     if (exitStatus != 0) {
            //         fprintf(stderr, "Failed to open file: %s\n", strerror(errno));
            //     }
            // } else {
            //     fprintf(stderr, "Failed to open file: %s\n", strerror(errno));
            // }
            break;
        }
        freeCharArr(args);
    }
}

void handleChDirCommand(char *originalLine) {
    char *curLine = originalLine;
    char *command;
    char *args[MAX_ARGS];

    if ((command = strsep(&curLine, " ")) != NULL) {
        size_t i = 0;
        args[i] = strdup(command);

        char *arg;
        while (curLine != NULL && (arg = strsep(&curLine, " ")) != NULL) {
            ++i;
            args[i] = strdup(arg);
        }


        if (i == 0 || i > 1) {
            fprintf(stderr, "An error has occurred\n");
        } else {
            // TODO: validate the path before proceeding chdir command
            switch (chdir(args[1]))
            {
            case -1:
                errThrow();
                break;
            
            default:
                break;
            }
        }

        freeCharArr(args);
    }
}

void handleInteractiveMode(char *argv[]) {
    const char *program = argv[0];

    char *programName = getProgramName(program);

    if (programName != NULL) {
        char *line = NULL;
        size_t len = 0;
        while (1) {
            printf("%s>", programName);
            
            if ((getline(&line, &len, stdin)) != -1) {
                // remove spaces and format the input line
                char formatedLine[MAX_LINE];
                formatLine(line, formatedLine);

                char *originalLine = strdup(formatedLine);
                char *curLine = originalLine;

                char *command;
                if (curLine != NULL && (command = strsep(&curLine, " ")) != NULL) {
                    char *commandLine = strdup(formatedLine);
                    if (strCmp(command, EXIT_COMMAND) == 0) {
                        free(commandLine);
                        break;
                    } else if (strCmp(command, CHDIR_COMMAND) == 0) {
                        handleChDirCommand(commandLine);
                    } else if (strCmp(command, PATH_COMMAND) == 0) {
                        printf("built-in command: %s\n", command);
                    } else {
                        handleUserCommand(commandLine);
                    }
                    free(commandLine);
                }

                free(originalLine);
            }  
        }
        if (line) {
            free(line);
        }
        free(programName);
    }
}





int main(int argc, char *argv[]) {

    const char *program = argv[0];

    char *programName = getProgramName(program);

    if (argc > 2) {
        errorExit(1);
    } else if (argc == 2) {
        char *filePath = argv[1];
        FILE *fileStream;

        fileStream = fopen(filePath, "r");

        if (fileStream == NULL) {
            errorExit(1);
        }
        
        size_t n = 0;
        char *line = NULL;
        char *commandLines[MAX_LINE];
        
        while ((getline(&line, &n, fileStream)) != -1) {
            // remove spaces and format the input line
            char formatedLine[MAX_LINE];
            formatLine(line, formatedLine);

            char *originalLine = strdup(formatedLine);
            char *curLine = originalLine;

            char *command;
            if (curLine != NULL && (command = strsep(&curLine, " ")) != NULL) {
                char *commandLine = strdup(formatedLine);
                if (strCmp(command, EXIT_COMMAND) == 0) {
                    free(commandLine);
                    break;
                } else if (strCmp(command, CHDIR_COMMAND) == 0) {
                    handleChDirCommand(commandLine);
                } else if (strCmp(command, PATH_COMMAND) == 0) {
                    printf("built-in command: %s\n", command);
                } else {
                    handleUserCommand(commandLine);
                }
                free(commandLine);
            }

            free(originalLine);
        }
        
        if (line) {
            free(line);
        }
        freeCharArr(commandLines);
        fclose(fileStream);
    } else {
        handleInteractiveMode(argv);
    }

    
    free(programName);

    return 0;
}