#ifndef WISH_H
#define WISH_H

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/wait.h>
#include <errno.h>

#define EXIT_COMMAND "exit"
#define CHDIR_COMMAND "cd"
#define PATH_COMMAND "path"
#define USR_EXE_PATH "/usr/bin/"
#define BIN_EXE_PATH "/bin/"

#define INTERACTIVE_MODE "interactiveMode"
#define BATCH_MODE "batchMode"

#endif