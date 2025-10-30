#ifndef COMMON_INCLUDE
#define COMMON_INCLUDE

#include <stdio.h>
#include <stdlib.h>

#define NELEMS(a) ( (sizeof(a)) / (sizeof(a[0])) )


extern void GetPrgName(const char *programArg, char *programName);


#endif