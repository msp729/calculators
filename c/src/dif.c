#ifndef DIFGUARD
#define DIFGUARD

#ifndef MAIN
#define MAIN
#define NUMMAIN
#endif

#include "num.c"

typedef struct {
    num x, dx;
} dif;

const dif d_init(num x, num dx) {
    dif ret;
    ret.x = x;
    ret.dx = dx;
    return ret;
}

const dif d_const(num x) { return d_init(x, n_new((int64_t)0)); }

#define d_new(X) d_const(_Generic((X), int64_t: n_z, double: n_r, complex: n_c)(X))

const dif d_add(dif a, dif b) { return d_init(n_add(a.x, b.x), n_add(a.dx, b.dx)); }

const dif d_neg(dif x) { return d_init(n_neg(x.x), n_neg(x.dx)); }

const dif d_mul(dif a, dif b) {return d_init(n_mul(a.x, b.x), n_add(n_mul(a.dx,b.x),n_mul(a.x, b.dx))); }

const dif d_recip(dif a) { return d_init(n_recip(a.x), n_neg(n_recip(n_mul(a.dx, a.dx)))); }

const dif d_div(dif a, dif b) { return d_init(n_div(a.x, b.x), n_div(n_sub(n_mul(a.dx, b.x), n_mul(b.dx, a.x)), n_mul(b.x, b.x))); }

const dif d_ln(dif a) { return d_init(n_ln(a.x), n_div(a.dx, a.x)); }

const dif d_exp(dif a) { return d_init(n_exp(a.x), n_mul(a.dx, n_exp(a.x))); }

const dif d_pow(dif a, dif b) { return d_exp(d_mul(d_ln(a), b)); }

const dif d_sin(dif x) { return d_init(n_sin(x.x), n_mul(x.dx, n_cos(x.x))); }

const dif d_cos(dif x) { return d_init(n_cos(x.x), n_mul(x.dx, n_neg(n_sin(x.x)))); }

#endif
