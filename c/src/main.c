#include <stdbool.h>
#include <string.h>
#define MAIN

#include "dif.c"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>

#define GET1 op1 = calculate(ans, line)
#define GET2 op2 = calculate(ans, line)
#define GET(n) *line += n; GET1; GET2

/// what one might expect
bool iswhitespace(char);
/// is the first argument a prefix of the second?
/// if so, return the length of the first argument.
int strpre(const char *pre, const char *str);
/// accept a newton's method command from a pointer to a char*
dif command(dif ans, char **);
/// accept a value from a pointer to a char*
dif calculate(dif ans, char **);
/// parse a function call
dif fcall(dif, char **);
/// output (wd is "should you print the derivative?")
void write(dif, bool wd);
/// output (wd is "should you print the derivative?")
void writeln(dif, bool wd);
/// main function
int main(void);

bool iswhitespace(char c) {
    return strchr(" \t\r\n", c) != NULL;
}

int strpre(const char* prefix, const char* string) {
    int i = -1;
    while (prefix[++i] != '\0')
        if (prefix[i] != string[i])
            return 0;
    return i;
}

dif command(dif ans, char **line) {
  if (**line == '0') {
    (*line)++;
    dif cur = calculate(ans, line);
    cur.dx = n_r(1.0);
    int limit = 999;
    while (limit > 0) {
      char *line2 = *line;
      dif fx = calculate(cur, &line2);
      if (n_mag2(fx.x) < 1e-12)
        break;
      cur = d_sub(cur, d_const(n_div(fx.x, fx.dx)));
      limit--;
    }
    if (limit == 0)
      printf("Error: ran into iteration limit.");
    return cur;
  } else
    printf("commands other than :0 not implemented yet"); // TODO
  return ans;
}

dif calculate(dif ans, char** line) {
    while (iswhitespace(**line) && **line != '\0') (*line)++;
    if (**line == '\0') return ans;
    dif op1, op2;
    switch (**line) {
        case '+': GET(1); return d_add(op1, op2);
        case '-': GET(1); return d_sub(op1, op2);
        case '*': GET(1); return d_mul(op1, op2);
        case '/': GET(1); return d_div(op1, op2);
        case '^': GET(1); return d_pow(op1, op2);
        case '_': (*line)++; GET1; return d_neg(op1);
        case '.': (*line)++; return ans;
        case 'j': (*line)++; return d_const(n_c(c_new(0.0, 1.0)));
    }
    double d;
    int n;
    if (sscanf(*line, "%lf%n", &d, &n) == 1) {
        *line += n;
        return d_new(d);
    }
    return fcall(ans, line);
}

#define FCALLCASE(s) FCALLCASEG(s, d_##s(v))

#define FCALLCASEG(s, ex)                                                      \
  if ((n = strpre(#s, *line)) != 0) {                                          \
    *line += n;                                                                \
    dif v = calculate(ans, line);                                              \
    return ex;                                                                 \
  }

dif fcall(dif ans, char **line) {
  int n;
  FCALLCASE(exp) FCALLCASE(ln);
  FCALLCASE(sinh) FCALLCASE(cosh) FCALLCASE(tanh);
  FCALLCASE(sin) FCALLCASE(cos) FCALLCASE(tan);
  FCALLCASE(asinh) FCALLCASE(acosh) FCALLCASE(atanh);
  FCALLCASE(asin) FCALLCASE(acos) FCALLCASE(atan);
  FCALLCASEG(lb, d_div(d_ln(v), d_ln(d_const(n_r(2.0)))));
  FCALLCASEG(lg, d_div(d_ln(v), d_ln(d_const(n_r(10.0)))));
  return ans;
}

void writeln(dif d, bool wd) {
  write(d, wd);
  printf("\n");
}

void write(dif d, bool wd) {
  num x = d.x;
  switch (x.tag) {
  case R:
    printf("%lf", x.val.r);
    break;
  case C:
    printf("%lf + %lfi", x.val.c.r, x.val.c.i);
    break;
  }
  x = d.dx;
  if (wd)
  switch (x.tag) {
  case R:
    printf("\t%lf", x.val.r);
    break;
  case C:
    printf("\t%lf + %lfi", x.val.c.r, x.val.c.i);
    break;
  }
}

int main(void) {
  printf("Hello World!\n");
  dif ans = d_new(0.0);
  while (true) {
    char *line = readline("c> ");
    if (line == NULL)
      return 0;
    int len = strlen(line);
    if (len == 0)
      continue;
    while (iswhitespace(line[0]))
      line++;
    if (line[0] == ':') {
      line++;
      writeln(ans = command(ans, &line), false);
    } else
      writeln(ans = calculate(ans, &line), false);
  }
}
