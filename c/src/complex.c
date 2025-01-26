#ifndef COMPLEXGUARD
#define COMPLEXGUARD

#ifndef MAIN
#define MAIN
#define COMPLEXMAIN
#endif

#include <math.h>
#include <stdbool.h>

#define RETC(real, imag)                                                       \
  complex ret;                                                                 \
  ret.r = real;                                                                \
  ret.i = imag;                                                                \
  return ret;

typedef struct complex {
  double r, i;
} complex;

const complex c_new(double a, double b) { RETC(a, b) }

const complex I = {.i = 1};
const complex ONE = {.r = 1};
const complex ZERO = {};

const bool eps(double a, double b) {
  double e = nextafter(a, INFINITY);
  double u = (e - a) * 1000 * 1000 * 10;
  return a - u < b && b < a + u;
}

const bool eq(complex a, complex b) { return eps(a.r, b.r) && eps(a.i, b.i); }

/// square magnitude
const double c_mag2(complex a) { return a.r * a.r + a.i * a.i; }

/// addition
const complex c_add(complex a, complex b) { RETC(a.r + b.r, a.i + b.i) }

/// negation
const complex c_neg(complex a) { RETC(-a.r, -a.i) }

const complex c_sub(complex a, complex b) { RETC(a.r - b.r, a.i - b.i) }

/// multiplication
const complex c_mul(complex a, complex b) {
  RETC(a.r * b.r - a.i * b.i, a.r * b.i + a.i * b.r)
}

/// reciprocal
const complex c_recip(complex a) {
  double m = c_mag2(a);
  RETC(a.r / m, -a.i / m);
}

/// division, by multiplication by reciprocal
const complex c_div(complex a, complex b) { return c_mul(a, c_recip(b)); }

/// exponential, base e
const complex c_exp(complex a) {
  double s = exp(a.r);
  RETC(s * cos(a.i), s * sin(a.i))
}

/// natural logarithm
const complex c_ln(complex a) { RETC(log(c_mag2(a)) / 2, atan2(a.i, a.r)) }

/// complex sine
const complex c_sin(complex a) {
  double s = exp(a.i);
  RETC(sin(a.r) * (s + 1 / s) / 2, cos(a.r) * (s - 1 / s) / 2)
}

/// complex cosine
const complex c_cos(complex a) {
  double s = exp(a.i);
  RETC(cos(a.r) * (s + 1 / s) / 2, sin(a.r) * (1 / s - s) / 2)
}

// s = e^ix
// s² -2iys - 1 = 0
// s = iy ± sqrt(1-y²)
// x = -i ln(iy±sqrt(1-y²))

const complex c_square(complex a) { RETC(a.r * a.r - a.i * a.i, 2 * a.r * a.i) }

const complex c_sqrt(complex a) {
  if (eq(a, ZERO))
    return ZERO;
  return c_exp(c_div(c_ln(a), c_new(2, 0)));
}

const complex c_asin(complex a) {
  return c_mul(c_neg(I),
               c_ln(c_add(c_mul(I, a), c_sqrt(c_sub(ONE, c_square(a))))));
}

#ifdef COMPLEXMAIN
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

bool testeq(complex a, complex b, complex c) {
  if (!eq(b, c)) {
    printf("\ttest failed, in: %lf + %lfi, left: %lf + %lfi, right: %lf + %lfi",
           a.r, a.i, b.r, b.i, c.r, c.i);
    return false;
  }
  return true;
}

double better() { return (rand() / (double)RAND_MAX) * 200 - 100; }

complex c_rand() { RETC(rand() / (double)RAND_MAX, rand() / (double)RAND_MAX) }

int main(void) {
  srand(time(NULL));
  printf("sin(x) = (e^ix - e^-ix) / 2i:\n");
  complex twoi = {.i = 2};
  int max = 1000 * 1000;
  for (int i = 1; i <= max; i++) {
    complex z = c_rand();
    complex iz = c_mul(I, z);
    if (!testeq(z, c_sin(z), c_div(c_sub(c_exp(iz), c_exp(c_neg(iz))), twoi)))
      break;
    else
      printf("\tPassed test %d/%d\r", i, max);
  }
  printf("\ncos(x) = (e^ix + e^-ix) / 2:\n");
  complex two = {.r = 2};
  for (int i = 1; i <= max; i++) {
    complex z = c_rand();
    complex iz = c_mul(I, z);
    if (!testeq(z, c_cos(z), c_div(c_add(c_exp(iz), c_exp(c_neg(iz))), two)))
      break;
    else
      printf("\tPassed test %d/%d\r", i, max);
  }
  printf("\n");
}

#endif
#endif
