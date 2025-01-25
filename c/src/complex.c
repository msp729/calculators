#ifndef GUARDCOMPLEX
#define GUARDCOMPLEX

#include <math.h>

#define RETC(real, imag)                                                       \
  complex ret;                                                                 \
  ret.r = real;                                                                \
  ret.i = imag;                                                                \
  return ret;

typedef struct complex {
  double r, i;
} complex;

const complex i = {.r = 0, .i = 1};
const complex one = {.r = 1, .i = 0};

double mag2(complex a) { return a.r * a.r + a.i * a.i; }

complex add(complex a, complex b){RETC(a.r + b.r, a.i + b.i)}

complex neg(complex a){RETC(-a.r, -a.i)}

complex
    mul(complex a, complex b){RETC(a.r * b.r - a.i * b.i, a.r *b.i + a.i * b.r)}

complex recip(complex a) {
    double m = mag2(a);
    RETC(a.r / m, -a.i / m);
}

complex div(complex a, complex b) {
    return mul(a,recip(b));
}

complex expc(complex a) {
    double s = exp(a.r);
    RETC(s*cos(a.i), s*sin(a.i))
}

#endif
