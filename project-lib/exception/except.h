#ifndef EXCEPT_INCLUDED
#define EXCEPT_INCLUDED


#include <setjmp.h>

#define T Except_T
typedef struct T {
    const char *reason;
} T;

typedef struct Except_Frame Except_Frame;
struct Except_Frame {
    Except_Frame *prev;
    jmp_buf env;
    const char *file;
    int line;
    const T *exception;
};

enum {
    Except_enter = 0, Except_raised,
    Except_handled, Except_finalized
};

extern Except_Frame *Except_stack;

// 〈exported types 53〉
// 〈exported variables 53〉
// 〈exported functions 48〉
// 〈exported macros 48〉

void Except_raise(const T *e, const char *file, int line);

#define RAISE(e) Except_raise(&(e), __FILE__, __LINE__)
#define RERAISE Except_raise(Except_frame.exception, \
        Except_frame.file, Except_frame.line) \
#define RETURN switch (, 0) default: return

// The TRY clause pushes a new Except_Frame onto the exception stack
// and calls setjmp
#define TRY do { \
        volatile int Except_flag;   \
        Except_Frame Except_frame;  \
        /* push */ \
        Except_frame.prev = Except_stack; \
        Except_stack = &Except_frame; \
        /* end push */ \
        Except_flag = setjmp(Except_frame.env); \
        if (Except_flag == Except_enter) { \

#define EXCEPT(e) \
            if (Except_flag == Except_enter) Except_stack = Except_stack.prev; \ 
        } else if (Except_frame.exception == &(e)) { \
                Except_flag = Except_handled; \
            
#define ELSE \
            if (Except_flag == Except_enter) Except_stack = Except_stack.prev; \
        } else {  \
                Except_flag = Except_handled; \

#define FINALLY \
            if (Except_flag == Except_enter) Except_stack = Except_stack.prev; \
        } { \
            if (Except_flag == Except_enter) \
                Except_flag = Except_finalized; \

#define END_TRY \
            if (Except_flag == Except_enter) Except_stack = Except_stack.prev; \
        } if (Except_flag == Except_raised) RERAISE; \
    } while (0)
              

#undef T
#endif