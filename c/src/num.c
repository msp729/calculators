#ifndef NUMGUARD
#define NUMGUARD

#ifndef MAIN
#define MAIN
#define NUMMAIN
#endif

#include "complex.c"
#include <stdint.h>

#define n_new(X) _Generic((X), int64_t: n_z, double: n_r, complex: n_c)(X)
#define DBG(NUM)                                                               \
  printf("tag: %d, z: %ld, r: %f, c: %f + i%f\n", NUM.tag, NUM.val.z,          \
         NUM.val.r, NUM.val.c.r, NUM.val.c.i)

#define NMATCH(X, Zv, Rv, Cv)                                                  \
  int64_t z;                                                                   \
  double r;                                                                    \
  complex c;                                                                   \
  switch (X.tag) {                                                             \
  case Z:                                                                      \
    z = X.val.z;                                                               \
    return Zv;                                                                 \
  case R:                                                                      \
    r = X.val.r;                                                               \
    return Rv;                                                                 \
  case C:                                                                      \
    c = X.val.c;                                                               \
    return Cv;                                                                 \
  }

#define SNM(X, Rf, Cf) NMATCH(X, n_r(Rf((double)z)), n_r(Rf(r)), n_c(Cf(c)))
#define CNM(X, Rc, Rf, Cf)                                                     \
  int64_t z;                                                                   \
  double r;                                                                    \
  complex c;                                                                   \
  num n = X;                                                                   \
  switch (n.tag) {                                                             \
  case Z:                                                                      \
    z = n.val.z;                                                               \
    n.val.r = (double)z;                                                       \
  case R:                                                                      \
    r = n.val.r;                                                               \
    if (Rc)                                                                    \
      return n_new(Rf);                                                        \
    n.val.c = c_new(r, 0);                                                     \
  case C:                                                                      \
    c = n.val.c;                                                               \
    return n_new(Cf);                                                          \
  }

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

const double Re(num x) { NMATCH(x, (double)z, r, c.r) }
const complex esc(num x) { NMATCH(x, c_new((double)z, 0.0), c_new(r, 0.0), c); }

const num n_add(num a, num b) {
  num ret;
  ret.tag = a.tag > b.tag ? a.tag : b.tag;
  switch (ret.tag) {
  case Z:
    ret.val.z = a.val.z + b.val.z;
  case R:
    ret.val.r = Re(a) + Re(b);
  case C:
    ret.val.c = c_add(esc(a), esc(b));
  }
  return ret;
}

const num n_neg(num a) { NMATCH(a, n_z(-z), n_r(-r), n_c(c_neg(c))) }

const num n_sub(num x, num y) { return n_add(x, n_neg(y)); }

const num n_recip(num a) {
  NMATCH(a, n_r(1 / (double)z), n_r(1 / r), n_c(c_recip(c)))
}

const num n_mul(num a, num b) {
  num ret;
  ret.tag = a.tag > b.tag ? a.tag : b.tag;
  switch (ret.tag) {
  case Z:
    ret.val.z = a.val.z * b.val.z;
  case R:
    ret.val.r = Re(a) * Re(b);
  case C:
    ret.val.c = c_mul(esc(a), esc(b));
  }
  return ret;
}

const num n_div(num x, num y) { return n_mul(x, n_recip(y)); }

const num n_exp(num x) {
  NMATCH(x, n_r(exp((double)z)), n_r(exp(r)), n_c(c_exp(c)))
}

const num n_ln(num x) {
  NMATCH(x, z > 0 ? n_r(log((double)z)) : n_c(c_new(log(-(double)z), M_PI)),
         r > 0 ? n_r(log(r)) : n_c(c_new(log(-r), M_PI)), n_c(c_ln(c)))
}

const num n_sin(num x) { SNM(x, sin, c_sin) }
const num n_cos(num x) { SNM(x, cos, c_cos) }
const num n_tan(num x) { SNM(x, tan, c_tan) }

const num n_sinh(num x) { SNM(x, sinh, c_sinh) }
const num n_cosh(num x) { SNM(x, cosh, c_cosh) }
const num n_tanh(num x) { SNM(x, tanh, c_tanh) }

const num n_asin(num x) { CNM(x, fabs(r) <= 1.0, asin(r), c_asin(c)) }
const num n_acos(num x) { CNM(x, fabs(r) <= 1.0, acos(r), c_acos(c)) }
const num n_atan(num x) { SNM(x, atan, c_atan) }

const num n_asinh(num x) { SNM(x, asinh, c_asinh) }
const num n_acosh(num x) { CNM(x, r >= 1.0, acosh(r), c_acosh(c)) }
const num n_atanh(num x) { CNM(x, fabs(r) <= 1.0, atanh(r), c_atanh(c)) }

#ifdef NUMMAIN

#include <stdio.h>

int main(void) { printf("num.c"); }

#endif
#endif
