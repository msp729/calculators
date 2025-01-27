#ifndef NUMGUARD
#define NUMGUARD

#ifndef MAIN
#define MAIN
#define NUMMAIN
#endif

#include "complex.c"
#include <stdint.h>

#define n_new(x) _Generic((X), int64_t: n_z, double: n_r, complex: n_c)(X)
#define DBG(NUM) printf("tag: %d, z: %lld, r: %f, c: %f + i%f\n", NUM.tag, NUM.val.z, NUM.val.r, NUM.val.c.r, NUM.val.c.i)

typedef union intern {
  int64_t z;
  double r;
  complex c;
} intern;
typedef enum tag { Z = 0, R = 1, C = 2 } tag;
typedef struct num {
  intern val;
  tag tag;
} num;

const num NBLANK = {.tag = Z, .val = {.z = 0}};

const num n_z(int64_t x) {
  num ret;
  ret.tag = Z;
  ret.val.z = x;
  return ret;
}

const num n_r(double x) {
  num ret;
  ret.tag = R;
  ret.val.r = x;
  return ret;
}

const num n_c(complex x) {
  num ret;
  ret.tag = C;
  ret.val.c = x;
  return ret;
}

void n_zero(num *x) {
  switch (x->tag) {
  case Z:
    x->val.z = 0;
    break;
  case R:
    x->val.r = 0;
    break;
  case C:
    x->val.c = ZERO;
    break;
  }
}

void n_one(num*x) {
    switch (x-> tag) {
        case Z: x->val.z=1;
        case R: x->val.r=1;
        case C: x->val.c=ONE;
    }
}

const num n_add(num a, num b) {
  num ret;
  ret.tag = a.tag < b.tag ? a.tag : b.tag;
  n_zero(&ret);
  // TODO
  return ret;
}

#ifdef NUMMAIN

#include <stdio.h>

int main(void) {
  for (tag tag = 0; tag <= 2; tag++) {
    num num = NBLANK;
    DBG(num);
    num.tag = tag;
    n_one(&num);
    DBG(num);
  }
}

#endif
#endif
