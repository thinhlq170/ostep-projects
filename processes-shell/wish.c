#include "wish.h"

#define MAX_LINE 1024
#define MAX_ARGS 128
#define MAX_WORD 64

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

char *getAccessedPath(char *command) {
    const char *dupCommand = strdup(command);
    char *binExePath = strdup(BIN_EXE_PATH);
    char *usrExePath = strdup(USR_EXE_PATH);

    binExePath = strcat(binExePath, dupCommand);
    usrExePath = strcat(usrExePath, dupCommand);

    int isBinExe = access(binExePath, X_OK);
    int isUsrExe = access(usrExePath, X_OK);

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
        free(arr[i]);
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
        while ((arg = strsep(&curLine, " ")) != NULL) {
            args[i] = strdup(arg);
            i++;
        }
        args[i] = NULL; // to mark end of arguments
        char *newEnv[] = { NULL };

        char *exePath = getAccessedPath(command);

        if (exePath != NULL) {
            pid_t childPId;
        
            switch (childPId = fork())
            {
            case -1:
                fprintf(stderr, "Fork fail. Cannot execute the command\n");
                break;
            case 0:
                if (isRedirectedCommand(args, i) == 0) {
                    // redirect standard output to the target file
                    close(STDOUT_FILENO);
                    open(args[i-1], O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
                    char *commandArgs[MAX_ARGS];
                    // take the command before the directed mark ">"
                    size_t j = 0;
                    while (j < i - 2) {
                        commandArgs[j] = strdup(args[j]);
                        ++j;
                    }
                    commandArgs[j] = NULL;
                    execve(exePath, commandArgs, newEnv);
                    freeCharArr(commandArgs);
                } else {
                    execve(exePath, args, newEnv);
                }
                    
                break;
            default:

                int commandStatus;

                waitpid(childPId, &commandStatus, 0);
                if (WIFEXITED(commandStatus)) {
                    int exitStatus = WEXITSTATUS(commandStatus);
                    if (exitStatus != 0) {
                        fprintf(stderr, "Fail command execution. Error code: %d\n", exitStatus);
                    }
                } else {
                    printf("Child process did not exit normally\n");
                }
                break;
            }
            free(exePath);
        } else {
            printf("%s: command not found\n", command);
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
        while ((arg = strsep(&curLine, " ")) != NULL) {
            ++i;
            args[i] = strdup(arg);
        }


        if (i == 0 || i > 1) {
            fprintf(stderr, "%s <path>\n", command);
        } else {
            // TODO: validate the path before proceeding chdir command
            switch (chdir(args[1]))
            {
            case -1:
                fprintf(stderr, "Chang directory fail\n");
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
            printf("%s> ", programName);
            
            if ((getline(&line, &len, stdin)) != -1) {

                // remove spaces and format the input line
                char formatedLine[MAX_LINE];
                formatLine(line, formatedLine);

                char *originalLine = strdup(formatedLine);
                char *curLine = originalLine;

                char *command;
                if ((command = strsep(&curLine, " ")) != NULL) {
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