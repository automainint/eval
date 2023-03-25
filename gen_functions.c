#include <inttypes.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

#ifndef M_PI
#  define M_PI 3.14159265358979323846
#endif

#ifndef M_E
#  define M_E 2.71828182845904523536
#endif

int64_t const scale = 0x1000000;
int64_t const range = 0x1000;

void print_func(FILE *out, long double (*const func)(long double),
                int64_t const left, int64_t const right,
                long double const x0, long double const x1,
                char const *const name) {
  fprintf(out, "static long long const eval_data_%s[%" PRId64 "] = {",
          name, range + 1 - left - right);
  for (int64_t i = left, k = 0; i <= range - right; i++, k++) {
    long double x = x0 + (((long double) i) * (x1 - x0)) /
                             ((long double) range);
    long double y = func(x);
    int64_t     z;
    if (y != y)
      z = 0;
    else
      z = (int64_t) floorl(y * ((long double) scale) + 0.5);
    if ((k % 20) == 0)
      fprintf(out, "\n ");
    fprintf(out, " %" PRId64 ",", z);
  }
  fprintf(out, "};\n\n");
}

int main(int argc, char **argv) {
  FILE *f = fopen("source/eval/data.inl.h", "w");
  fprintf(f, "#define EVAL_SCALE %" PRId64 "\n", scale);
  fprintf(f, "#define EVAL_RANGE %" PRId64 "\n\n", range);
  fprintf(f, "#define EVAL_SQRT_RANGE eval_imul(10, scale)\n");
  fprintf(f, "#define EVAL_LOG_RANGE  eval_imul(10, scale)\n");
  fprintf(f, "#define EVAL_EXP_MIN    eval_imul(-10, scale)\n");
  fprintf(f, "#define EVAL_EXP_RANGE  eval_imul(20, scale)\n");
  fprintf(f, "#define EVAL_SIN_RANGE  eval_pi(scale)\n");
  fprintf(f, "#define EVAL_TAN_RANGE  eval_pi_2(scale)\n");
  fprintf(f, "#define EVAL_ASIN_RANGE scale\n");
  fprintf(f, "#define EVAL_ATAN_RANGE eval_imul(10, scale)\n\n");
  print_func(f, sqrtl, 0, 0, 0.0, 10.0, "sqrt");
  print_func(f, logl, 1, 0, 0.0, 10.0, "log");
  print_func(f, expl, 0, 0, -10.0, 10.0, "exp");
  print_func(f, sinl, 0, 0, 0.0, M_PI, "sin");
  print_func(f, tanl, 0, 1, 0.0, M_PI / 2, "tan");
  print_func(f, asinl, 0, 0, 0.0, 1.0, "asin");
  print_func(f, atanl, 0, 0, 0.0, 10.0, "atan");
  fclose(f);
  return 0;
}
