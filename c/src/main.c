#include <stdbool.h>
#include <string.h>
#define MAIN

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "dif.c"

#define GET1 op1 = calculate(ans, line)
#define GET2 op2 = calculate(ans, line)
#define GET(n) *line += n; GET1; GET2

bool iswhitespace(char);
int strpre(const char *, const char *);
void command(char **);
dif calculate(dif, char **);
dif fcall(dif, char **);
void writeln(dif);
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

void command(char **line) {
  if (**line == '0') {
    (*line)++;
    dif cur = calculate(d_new(0.0), line);
    cur.dx = n_r(1.0);
    int limit = 999;
    while (limit > 0) {
      char *line2 = *line;
      dif fx = calculate(cur, &line2);
      if (n_mag2(fx.x) < 1e-12)
        break;
      cur = d_sub(cur, d_const(n_div(fx.x, fx.dx)));
      limit--;
      writeln(cur);
      writeln(fx);
    }
    if (limit == 0)
      printf("Error: ran into iteration limit.");
    writeln(cur);
  } else
    printf("commands other than :0 not implemented yet"); // TODO
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

void writeln(dif d) {
  num x = d.x;
  switch (x.tag) {
  case R:
    printf("%lf\t", x.val.r);
    break;
  case C:
    printf("%lf + %lfi\t", x.val.c.r, x.val.c.i);
    break;
  }
  x = d.dx;
  switch (x.tag) {
  case R:
    printf("%lf\n", x.val.r);
    break;
  case C:
    printf("%lf + %lfi\n", x.val.c.r, x.val.c.i);
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
      command(&line);
    } else
      writeln(ans = calculate(ans, &line));
  }
}
