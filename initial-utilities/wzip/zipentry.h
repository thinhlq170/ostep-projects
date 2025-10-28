#ifndef ZIP_ENTRY_INCLUDE
#define ZIP_ENTRY_INCLUDE

// Based on David R Hanson's chapter 2

#define T Zip_T
typedef struct T *T;

struct T {
    int count;
    char character;
};

extern T Zip_new(char character);

#undef T
#endif