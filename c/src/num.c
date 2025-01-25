#ifndef GUARDNUM
#define GUARDNUM

#include <stdint.h>
#include "complex.c"

typedef union intern { int64_t z; double r; complex c; } intern;
typedef enum tag { Z, R, C } tag;
typedef struct num {intern val; tag tag;} num;

#endif
