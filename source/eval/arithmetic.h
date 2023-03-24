#ifndef EVAL_ARITHMETIC_H
#define EVAL_ARITHMETIC_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int64_t eval_integer_t;

#define EVAL_MAX INT64_MAX
#define EVAL_MIN -INT64_MAX
#define EVAL_MUL_SAFETY 0xb504f333ll

#define EVAL_ADD(x, y) \
  ((eval_integer_t) (((uint64_t) x) + ((uint64_t) y)))
#define EVAL_SUB(x, y) \
  ((eval_integer_t) (((uint64_t) x) - ((uint64_t) y)))
#define EVAL_MUL(x, y) \
  ((eval_integer_t) (((uint64_t) x) * ((uint64_t) y)))
#define EVAL_DIV(x, y) \
  (y == 0 ? 0 : ((eval_integer_t) (((uint64_t) x) / ((uint64_t) y))))

#ifdef __GNUC__
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wunused-function"
#  pragma GCC diagnostic ignored "-Wunknown-pragmas"
#  pragma GCC            push_options
#  pragma GCC            optimize("O3")
#endif

static eval_integer_t eval_neg(eval_integer_t const x) {
  if (x <= EVAL_MIN)
    return EVAL_MAX;
  return -x;
}

static eval_integer_t eval_add(eval_integer_t const x,
                               eval_integer_t const y) {
  if (y > 0 && x >= EVAL_MAX - y)
    return EVAL_MAX;
  if (y < 0 && x <= EVAL_MIN - y)
    return EVAL_MIN;
  return x + y;
}

static eval_integer_t eval_sub(eval_integer_t const x,
                               eval_integer_t const y) {
  if (y < 0 && x >= EVAL_MAX + y)
    return EVAL_MAX;
  if (y > 0 && x <= EVAL_MIN + y)
    return EVAL_MIN;
  return x - y;
}

static eval_integer_t eval_idiv(eval_integer_t const x,
                                eval_integer_t const y) {
  if (y != 0)
    return x / y;
  if (x < 0)
    return EVAL_MIN;
  if (x > 0)
    return EVAL_MAX;
  return 1;
}

static eval_integer_t eval_imul(eval_integer_t const x,
                                eval_integer_t const y) {
  if (x >= EVAL_MUL_SAFETY) {
    if (y > EVAL_MAX / x)
      return EVAL_MAX;
    if (y < EVAL_MIN / x)
      return EVAL_MIN;
  }
  if (x <= -EVAL_MUL_SAFETY) {
    if (y > EVAL_MIN / x)
      return EVAL_MIN;
    if (y < EVAL_MAX / x)
      return EVAL_MAX;
  }
  if (y >= EVAL_MUL_SAFETY) {
    if (x > EVAL_MAX / y)
      return EVAL_MAX;
    if (x < EVAL_MIN / y)
      return EVAL_MIN;
  }
  if (y <= -EVAL_MUL_SAFETY) {
    if (x > EVAL_MIN / y)
      return EVAL_MIN;
    if (x < EVAL_MAX / y)
      return EVAL_MAX;
  }
  return x * y;
}

static eval_integer_t eval_div(eval_integer_t const x,
                               eval_integer_t const y,
                               eval_integer_t const scale) {
  //  x / y = eval_div(x, y, scale) / scale
  //  eval_div(x, y, scale) = x * scale / y
  if (y == 0) {
    if (x > 0)
      return EVAL_MAX;
    if (x < 0)
      return EVAL_MIN;
    return scale;
  }

  if (scale < 0)
    return -eval_div(x, y, -scale);
  if (y < 0)
    return -eval_div(x, -y, scale);

  eval_integer_t const half_y = y / 2;

  if (x < 0) {
    if (x < EVAL_MIN / scale)
      return eval_imul(eval_idiv(eval_sub(x, half_y), y), scale);
    return eval_idiv(eval_sub(eval_imul(x, scale), half_y), y);
  }

  if (x > EVAL_MAX / scale)
    return eval_imul(eval_idiv(eval_add(x, half_y), y), scale);
  return eval_idiv(eval_add(eval_imul(x, scale), half_y), y);
}

static eval_integer_t eval_mul(eval_integer_t const x,
                               eval_integer_t const y,
                               eval_integer_t const scale) {
  //  (x / scale) * (y / scale) = (x * y / scale) / scale
  //                               ^^^^^^^^^^^^^
  return eval_div(x, scale, y);
}

static eval_integer_t eval_div_sum2(eval_integer_t const x,
                                    eval_integer_t const y,
                                    eval_integer_t const divisor) {
  //  (x + y) / divisor
  if ((y < 0 && x <= EVAL_MIN - y) || (y > 0 && x >= EVAL_MAX - y))
    return eval_add(eval_idiv(x, divisor), eval_idiv(y, divisor));
  return eval_idiv(x + y, divisor);
}

static eval_integer_t eval_div_sum3(eval_integer_t const x,
                                    eval_integer_t const y,
                                    eval_integer_t const z,
                                    eval_integer_t const divisor) {
  //  (x + y + z) / divisor
  eval_integer_t const sum_x_y = eval_add(x, y);
  if ((z < 0 && sum_x_y <= EVAL_MIN - z) ||
      (z > 0 && sum_x_y >= EVAL_MAX - z))
    return eval_add(
        eval_add(eval_idiv(x, divisor), eval_idiv(y, divisor)),
        eval_idiv(z, divisor));
  return eval_idiv(eval_add(sum_x_y, z), divisor);
}

#ifdef __GNUC__
#  pragma GCC            pop_options
#  pragma GCC diagnostic pop
#endif

#ifdef __cplusplus
}
#endif

#endif
