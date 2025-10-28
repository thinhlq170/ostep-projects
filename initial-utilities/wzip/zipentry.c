#include "zipentry.h"
#include "../../common.h"
#include "../../project-lib/mem/mem.h"

// Based on David R Hanson's chapter 2

#define T Zip_T

T Zip_new(char character) {
    T p;
    NEW(p);
    p->character = character;
    p->count = 1;

    return p;
}

#undef T