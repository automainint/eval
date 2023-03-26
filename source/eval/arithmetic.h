#ifndef EVAL_ARITHMETIC_H
#define EVAL_ARITHMETIC_H

#include "options.h"

#ifdef __cplusplus
extern "C" {
#endif

#define EVAL_ADD(x, y)                          \
  ((eval_integer_t) (((unsigned long long) x) + \
                     ((unsigned long long) y)))
#define EVAL_SUB(x, y)                          \
  ((eval_integer_t) (((unsigned long long) x) - \
                     ((unsigned long long) y)))
#define EVAL_MUL(x, y)                          \
  ((eval_integer_t) (((unsigned long long) x) * \
                     ((unsigned long long) y)))
#define EVAL_DIV(x, y)                                    \
  (y == 0 ? 0                                             \
          : ((eval_integer_t) (((unsigned long long) x) / \
                               ((unsigned long long) y))))

#ifdef __GNUC__
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wunused-function"
#  pragma GCC diagnostic ignored "-Wunknown-pragmas"
#  pragma GCC            push_options
#  pragma GCC            optimize("O3")
#endif

static eval_integer_t eval_abs(eval_integer_t const x) {
  return x < 0 ? -x : x;
}

static eval_integer_t eval_min(eval_integer_t const x,
                               eval_integer_t const y) {
  return x < y ? x : y;
}

static eval_integer_t eval_max(eval_integer_t const x,
                               eval_integer_t const y) {
  return x < y ? y : x;
}

static eval_integer_t eval_min3(eval_integer_t const x,
                                eval_integer_t const y,
                                eval_integer_t const z) {
  return eval_min(eval_min(x, y), z);
}

static eval_integer_t eval_max3(eval_integer_t const x,
                                eval_integer_t const y,
                                eval_integer_t const z) {
  return eval_max(eval_max(x, y), z);
}

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

static eval_integer_t eval_add3(eval_integer_t const x,
                                eval_integer_t const y,
                                eval_integer_t const z) {
  int xy_overflow = (y > 0 && x > EVAL_MAX - y) ||
                    (y < 0 && x < EVAL_MIN - y);
  int yz_overflow = (z > 0 && y > EVAL_MAX - z) ||
                    (z < 0 && y < EVAL_MIN - z);
  int xz_overflow = (z > 0 && x > EVAL_MAX - z) ||
                    (z < 0 && x < EVAL_MIN - z);
  if (xy_overflow && xz_overflow)
    return eval_add(eval_add(y, z), x);
  if (xy_overflow && yz_overflow)
    return eval_add(eval_add(z, x), y);
  return eval_add(eval_add(x, y), z);
}

static eval_integer_t eval_idiv(eval_integer_t const x_,
                                eval_integer_t const y_) {
  eval_integer_t const x = x_ < 0 ? -x_ : x_;
  eval_integer_t const y = y_ < 0 ? -y_ : y_;
  eval_integer_t       z;

  if (y == 0) {
    if (x == 0)
      z = 0;
    else
      z = EVAL_MAX;
  } else
    z = eval_add(x, y / 2) / y;

  return (x_ < 0) ^ (y_ < 0) ? -z : z;
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

static eval_integer_t eval_div(eval_integer_t const x_,
                               eval_integer_t const y_,
                               eval_integer_t const scale_) {
  //  x / y = eval_div(x, y, scale) / scale
  //  eval_div(x, y, scale) = (x / y) * scale

  if (scale_ == 0)
    return 0;

  if (y_ == 0) {
    if (x_ > 0)
      return EVAL_MAX;
    if (x_ < 0)
      return EVAL_MIN;
    return scale_;
  }

  eval_integer_t const x     = x_ < 0 ? -x_ : x_;
  eval_integer_t const y     = y_ < 0 ? -y_ : y_;
  eval_integer_t const scale = scale_ < 0 ? -scale_ : scale_;

  eval_integer_t z;

  if (x < EVAL_MAX / scale)
    //  (x * scale) / y
    z = eval_idiv(eval_imul(x, scale), y);
  else {
    eval_integer_t const x_div_y     = eval_idiv(x, y);
    eval_integer_t const y_div_scale = eval_idiv(y, scale);

    eval_integer_t loss_x, loss_y;

    if (scale < y) {
      loss_x = eval_abs(x - eval_imul(x_div_y, y));
      loss_y = eval_abs(y - eval_imul(y_div_scale, scale));
    }

    if (scale < y && loss_y < loss_x)
      //  x / (y / scale)
      z = eval_idiv(x, y_div_scale);
    else
      //  (x / y) * scale
      z = eval_imul(x_div_y, scale);
  }

  return (scale_ < 0) ^ (y_ < 0) ^ (x_ < 0) ? -z : z;
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
    return eval_add3(eval_idiv(x, divisor), eval_idiv(y, divisor),
                     eval_idiv(z, divisor));
  return eval_idiv(eval_add(sum_x_y, z), divisor);
}

static eval_integer_t eval_wrap(eval_integer_t const x,
                                eval_integer_t const x_min_,
                                eval_integer_t const x_range_) {
  eval_integer_t x_min   = x_min_;
  eval_integer_t x_range = x_range_;

  if (x_range_ == 0)
    return x_min_;

  if (x_range_ < 0) {
    x_min   = x_min_ + x_range_;
    x_range = -x_range_;
  }

  eval_integer_t const distance = x - x_min;
  eval_integer_t       repeats  = 0;

  if (distance < 0)
    repeats = (distance - x_range + 1) / x_range;
  else if (distance >= x_range)
    repeats = distance / x_range;

  return x - repeats * x_range;
}

static eval_integer_t eval_lerp(eval_integer_t const x0,
                                eval_integer_t const x1,
                                eval_integer_t const t,
                                eval_integer_t const scale) {
  if (scale == 0)
    return x0;

  if ((x0 < 0 && x1 > EVAL_MAX + x0) ||
      (x0 > 0 && x1 < EVAL_MIN + x0))
    /*  Subtraction (x1 - x0) will overflow.
     *  Use different order of operations.
     */
    //  x = x0 + (x1 * t / scale) + (-x0 * t / scale)
    return eval_add3(x0, eval_mul(x1, t, scale),
                     eval_neg(eval_mul(x0, t, scale)));

  //  x = x0 + (x1 - x0) * t / scale
  return eval_add(x0, eval_div(eval_sub(x1, x0), scale, t));
}

#ifdef __GNUC__
#  pragma GCC            pop_options
#  pragma GCC diagnostic pop
#endif

#ifdef __cplusplus
}
#endif

#endif
