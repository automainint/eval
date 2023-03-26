#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef M_PI
#  define M_PI 3.14159265358979323846
#endif

#ifndef M_E
#  define M_E 2.71828182845904523536
#endif

int const ALIGN_LINE  = 200;
int const ALIGN_CONST = 11;

int64_t const SCALE = 100000000000000ll;
int64_t const RANGE = 0x1000;

void print_const(FILE *const out, long double const value,
                 char const *const name) {
  fprintf(out, "#define EVAL_%s%*c%lldll\n", name,
          (int) (ALIGN_CONST - strlen(name)), ' ',
          (long long) (value * SCALE + 0.5));
}

void print_func(FILE *const out,
                long double (*const func)(long double),
                int64_t const left, int64_t const right,
                long double const x0, long double const x1,
                char const *const name) {
  char buf[200];
  sprintf(buf, "static long long const eval_data_%s[%lld] = {", name,
          (long long) (RANGE + 1 - left - right));
  sprintf(buf + strlen(buf), "%*c", (int) (22 - (strlen(buf) % 22)),
          ' ');
  int64_t k = strlen(buf);
  fprintf(out, "%s", buf);
  for (int64_t i = left; i <= RANGE - right; i++) {
    long double x = x0 + (((long double) i) * (x1 - x0)) /
                             ((long double) RANGE);
    long double y = func(x);
    int64_t     z;
    if (y != y)
      z = 0;
    else
      z = (int64_t) floorl(y * ((long double) SCALE) + 0.5);
    if (k >= ALIGN_LINE) {
      fprintf(out, "\n");
      k = 0;
    }
    sprintf(buf, "%19lldll", (long long) z);
    if (i < RANGE - right)
      sprintf(buf + strlen(buf), ",");
    k += strlen(buf);
    fprintf(out, "%s", buf);
  }
  fprintf(out, " };\n\n");
}

long double log2_(long double x) {
  return logl(x) / logl(2.0);
}

long double log10_(long double x) {
  return logl(x) / logl(10.0);
}

int main(int argc, char **argv) {
  FILE *f = fopen("./source/eval/data.inl.h", "w");

  fprintf(f, "#define EVAL_SCALE %lldll\n", (long long) SCALE);
  fprintf(f, "#define EVAL_RANGE %lldll\n", (long long) RANGE);
  fprintf(f, "\n");

  print_const(f, M_PI, "PI");
  print_const(f, M_PI / 2, "PI_2");
  print_const(f, M_PI / 4, "PI_4");
  print_const(f, M_PI * 2, "2_PI");
  print_const(f, M_PI * 4, "4_PI");
  print_const(f, M_E, "E");
  print_const(f, M_E / 2, "E_2");
  print_const(f, M_E / 4, "E_4");
  print_const(f, M_E * 2, "2_E");
  print_const(f, M_E * 4, "4_E");
  print_const(f, sqrt(M_E), "SQRT_E");
  print_const(f, sqrt(2.0), "SQRT_2");
  print_const(f, sqrt(3.0), "SQRT_3");
  print_const(f, sqrt(10.0), "SQRT_10");
  print_const(f, sqrt(100.0), "SQRT_100");
  print_const(f, log(2.0), "LOG_2");
  print_const(f, log(10.0), "LOG_10");
  print_const(f, log(100.0), "LOG_100");
  print_const(f, log2_(M_E), "LOG2_E");
  print_const(f, log2_(10.0), "LOG2_10");
  print_const(f, log2_(100.0), "LOG2_100");
  print_const(f, log10_(M_E), "LOG10_E");
  print_const(f, log10_(2.0), "LOG10_2");
  fprintf(f, "\n");

  fprintf(f, "#define EVAL_SQRT_RANGE   eval_imul(10, scale)\n");
  fprintf(f, "#define EVAL_LOG_RANGE    eval_imul(10, scale)\n");
  fprintf(f, "#define EVAL_LOG2_RANGE   eval_imul(10, scale)\n");
  fprintf(f, "#define EVAL_LOG10_RANGE  eval_imul(100, scale)\n");
  fprintf(f, "#define EVAL_EXP_MIN      eval_imul(-10, scale)\n");
  fprintf(f, "#define EVAL_EXP_RANGE    eval_imul(20, scale)\n");
  fprintf(f, "#define EVAL_SIN_RANGE    eval_pi_2(scale)\n");
  fprintf(f, "#define EVAL_TAN_RANGE    eval_pi_2(scale)\n");
  fprintf(f, "#define EVAL_ASIN_RANGE   scale\n");
  fprintf(f, "#define EVAL_ATAN_RANGE   eval_imul(10, scale)\n");
  fprintf(f, "\n");

  print_func(f, sqrtl, 0, 0, 0.0, 10.0, "sqrt");
  print_func(f, logl, 1, 0, 0.0, 10.0, "log");
  print_func(f, log2_, 1, 0, 0.0, 10.0, "log2");
  print_func(f, log10_, 1, 0, 0.0, 100.0, "log10");
  print_func(f, expl, 0, 0, -10.0, 10.0, "exp");
  print_func(f, sinl, 0, 1, 0.0, M_PI / 2, "sin");
  print_func(f, tanl, 0, 1, 0.0, M_PI / 2, "tan");
  print_func(f, asinl, 0, 0, 0.0, 1.0, "asin");
  print_func(f, atanl, 0, 0, 0.0, 10.0, "atan");

  fclose(f);
  return 0;
}
