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

void command(char* line) {
    printf("commands not implemented yet"); // TODO
}

dif fcall(dif ans, char** line);

dif calculate(dif ans, char** line) {
    if (**line == '\0') return ans;
    while (iswhitespace(**line)) (*line)++;
    dif op1, op2;
    switch (**line) {
        case '+': GET(1); return d_add(op1, op2);
        case '*': GET(1); return d_mul(op1, op2);
        case '/': GET(1); return d_div(op1, op2);
        case '^': GET(1); return d_pow(op1, op2);
        case '-': GET1; (*line)++; return d_neg(op1);
        case '.': (*line)++; return ans;
        case 'j': return d_const(n_c(c_new(0.0, 1.0)));
    }
    double d;
    int n;
    if (sscanf(*line, "%lf%n", &d, &n) == 1) {
        *line += n;
        return d_new(d);
    }
    return fcall(ans, line);
}

#define FCALLCASE(s)                                                           \
  if ((n = strpre(#s, *line)) != 0) {                                          \
    *line += n;                                                                \
    return d_##s(calculate(ans, line));                                        \
  } else

dif fcall(dif ans, char** line) {
    int n;
    FCALLCASE(sin)
    FCALLCASE(cos)
    FCALLCASE(exp)
    FCALLCASE(ln)
    return ans;
}

void writeln(dif d) {
    num x = d.x;
    switch (x.tag) {
    case Z:
        printf("%lli\n", x.val.z);
        break;
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
    dif ans = d_new((int64_t)0);
    while (true) {
        char* line = readline("c> ");
        if (line == NULL) return 0;
        int len = strlen(line);
        if (len == 0) continue;
        while (iswhitespace(line[0])) line++;
        if (line[0] == ':') command(line+1);
        else writeln(ans = calculate(ans, &line));
    }
}
