#include "functions.h"

#include "arithmetic.h"
#include "data.inl.h"
#include <assert.h>

#define EVAL_UPSCALE 10

eval_integer_t eval_pi(eval_integer_t scale) {
  return eval_div(EVAL_PI, EVAL_SCALE, scale);
}

eval_integer_t eval_pi_2(eval_integer_t scale) {
  return eval_div(EVAL_PI_2, EVAL_SCALE, scale);
}

eval_integer_t eval_pi_4(eval_integer_t scale) {
  return eval_div(EVAL_PI_4, EVAL_SCALE, scale);
}

eval_integer_t eval_2_pi(eval_integer_t scale) {
  return eval_div(EVAL_2_PI, EVAL_SCALE, scale);
}

eval_integer_t eval_4_pi(eval_integer_t scale) {
  return eval_div(EVAL_4_PI, EVAL_SCALE, scale);
}

eval_integer_t eval_e(eval_integer_t scale) {
  return eval_div(EVAL_E, EVAL_SCALE, scale);
}

eval_integer_t eval_e_2(eval_integer_t scale) {
  return eval_div(EVAL_E_2, EVAL_SCALE, scale);
}

eval_integer_t eval_e_4(eval_integer_t scale) {
  return eval_div(EVAL_E_4, EVAL_SCALE, scale);
}

eval_integer_t eval_2_e(eval_integer_t scale) {
  return eval_div(EVAL_2_E, EVAL_SCALE, scale);
}

eval_integer_t eval_4_e(eval_integer_t scale) {
  return eval_div(EVAL_4_E, EVAL_SCALE, scale);
}

eval_integer_t eval_sqrt_e(eval_integer_t scale) {
  return eval_div(EVAL_4_E, EVAL_SCALE, scale);
}

eval_integer_t eval_sqrt_2(eval_integer_t scale) {
  return eval_div(EVAL_SQRT_2, EVAL_SCALE, scale);
}

eval_integer_t eval_sqrt_3(eval_integer_t scale) {
  return eval_div(EVAL_SQRT_3, EVAL_SCALE, scale);
}

eval_integer_t eval_sqrt_10(eval_integer_t scale) {
  return eval_div(EVAL_SQRT_10, EVAL_SCALE, scale);
}

eval_integer_t eval_sqrt_100(eval_integer_t scale) {
  return eval_div(EVAL_SQRT_100, EVAL_SCALE, scale);
}

eval_integer_t eval_log_2(eval_integer_t scale) {
  return eval_div(EVAL_LOG_2, EVAL_SCALE, scale);
}

eval_integer_t eval_log_10(eval_integer_t scale) {
  return eval_div(EVAL_LOG_10, EVAL_SCALE, scale);
}

eval_integer_t eval_log_100(eval_integer_t scale) {
  return eval_div(EVAL_LOG_100, EVAL_SCALE, scale);
}

eval_integer_t eval_log2_e(eval_integer_t scale) {
  return eval_div(EVAL_LOG2_E, EVAL_SCALE, scale);
}

eval_integer_t eval_log2_10(eval_integer_t scale) {
  return eval_div(EVAL_LOG2_10, EVAL_SCALE, scale);
}

eval_integer_t eval_log2_100(eval_integer_t scale) {
  return eval_div(EVAL_LOG2_100, EVAL_SCALE, scale);
}

eval_integer_t eval_log10_e(eval_integer_t scale) {
  return eval_div(EVAL_LOG10_E, EVAL_SCALE, scale);
}

eval_integer_t eval_log10_2(eval_integer_t scale) {
  return eval_div(EVAL_LOG10_2, EVAL_SCALE, scale);
}

/*  BASIC FUNCTIONS
 *
 *  Trigonometric and logarithmic functions defined by arrays with
 *  function values.
 */

static eval_integer_t eval_sqrt_fetch(eval_integer_t n) {
  if (n < 0)
    n = 0;
  if (n > EVAL_RANGE)
    n = EVAL_RANGE;

  assert(n >= 0);
  assert(n < sizeof eval_data_sqrt / sizeof *eval_data_sqrt);

  return eval_data_sqrt[n];
}

static eval_integer_t eval_log_fetch(eval_integer_t n) {
  if (n <= 0)
    return EVAL_MIN;
  if (n > EVAL_RANGE)
    n = EVAL_RANGE;

  assert(n > 0);
  assert(n < sizeof eval_data_log / sizeof *eval_data_log);

  return eval_data_log[n - 1];
}

static eval_integer_t eval_log2_fetch(eval_integer_t n) {
  if (n <= 0)
    return EVAL_MIN;
  if (n > EVAL_RANGE)
    n = EVAL_RANGE;

  assert(n > 0);
  assert(n < sizeof eval_data_log2 / sizeof *eval_data_log2);

  return eval_data_log2[n - 1];
}

static eval_integer_t eval_log10_fetch(eval_integer_t n) {
  if (n <= 0)
    return EVAL_MIN;
  if (n > EVAL_RANGE)
    n = EVAL_RANGE;

  assert(n > 0);
  assert(n < sizeof eval_data_log10 / sizeof *eval_data_log10);

  return eval_data_log10[n - 1];
}

static eval_integer_t eval_exp_fetch(eval_integer_t n) {
  if (n < 0)
    return 0;
  if (n > EVAL_RANGE)
    n = EVAL_RANGE;

  assert(n >= 0);
  assert(n < sizeof eval_data_exp / sizeof *eval_data_exp);

  return eval_data_exp[n];
}

static eval_integer_t eval_sin_fetch_wrapped(eval_integer_t n) {
  n = eval_wrap(n, -EVAL_RANGE, EVAL_RANGE * 2);

  assert(-n < sizeof eval_data_sin / sizeof *eval_data_sin);
  assert(n < sizeof eval_data_sin / sizeof *eval_data_sin);

  return n < 0 ? -eval_data_sin[-n] : eval_data_sin[n];
}

static eval_integer_t eval_tan_fetch_wrapped(eval_integer_t n) {
  n = eval_wrap(n, -EVAL_RANGE, EVAL_RANGE * 2);

  if (n <= -EVAL_RANGE)
    //  +/- infinity
    return 0;

  assert(-n < sizeof eval_data_tan / sizeof *eval_data_tan);
  assert(n < sizeof eval_data_tan / sizeof *eval_data_tan);

  return n < 0 ? -eval_data_tan[-n] : eval_data_tan[n];
}

static eval_integer_t eval_asin_fetch_sym(eval_integer_t n) {
  if (n < -EVAL_RANGE)
    n = -EVAL_RANGE;
  if (n > EVAL_RANGE)
    n = EVAL_RANGE;

  assert(-n < sizeof eval_data_asin / sizeof *eval_data_asin);
  assert(n < sizeof eval_data_asin / sizeof *eval_data_asin);

  return n < 0 ? -eval_data_asin[-n] : eval_data_asin[n];
}

static eval_integer_t eval_atan_fetch_sym(eval_integer_t n) {
  if (n < -EVAL_RANGE)
    return -EVAL_PI_2;
  if (n > EVAL_RANGE)
    return EVAL_PI_2;

  assert(-n < sizeof eval_data_atan / sizeof *eval_data_atan);
  assert(n < sizeof eval_data_atan / sizeof *eval_data_atan);

  return n < 0 ? -eval_data_atan[-n] : eval_data_atan[n];
}

#define INTERPOLATE_(fetch_, x_, x_range_, z_)   \
  do {                                           \
    eval_integer_t n0, n1, x0, x1;               \
                                                 \
    n0 = eval_div((x_), (x_range_), EVAL_RANGE); \
                                                 \
    if ((x_range_) < EVAL_RANGE) {               \
      z_ = fetch_(n0);                           \
      break;                                     \
    }                                            \
                                                 \
    x0 = eval_div(n0, EVAL_RANGE, (x_range_));   \
                                                 \
    if (x0 == (x_)) {                            \
      z_ = fetch_(n0);                           \
      break;                                     \
    }                                            \
                                                 \
    if (x0 < (x_)) {                             \
      n1 = n0 + 1;                               \
      x1 = eval_div(n1, EVAL_RANGE, (x_range_)); \
    } else {                                     \
      n1 = n0;                                   \
      x1 = x0;                                   \
      n0 = n1 - 1;                               \
      x0 = eval_div(n0, EVAL_RANGE, (x_range_)); \
    }                                            \
                                                 \
    if (x1 <= x0) {                              \
      z_ = fetch_(n0);                           \
      break;                                     \
    }                                            \
                                                 \
    eval_integer_t const z0 = fetch_(n0);        \
    eval_integer_t const z1 = fetch_(n1);        \
                                                 \
    z_ = eval_lerp(z0, z1, (x_) -x0, x1 - x0);   \
  } while (0)

static eval_integer_t eval_sqrt_0_10(eval_integer_t x,
                                     eval_integer_t x_range) {
  eval_integer_t z;
  INTERPOLATE_(eval_sqrt_fetch, x, x_range, z);
  return z;
}

static eval_integer_t eval_log_0_10(eval_integer_t x,
                                    eval_integer_t x_range) {
  eval_integer_t z;
  INTERPOLATE_(eval_log_fetch, x, x_range, z);
  return z;
}

static eval_integer_t eval_log2_0_10(eval_integer_t x,
                                     eval_integer_t x_range) {
  eval_integer_t z;
  INTERPOLATE_(eval_log2_fetch, x, x_range, z);
  return z;
}

static eval_integer_t eval_log10_0_100(eval_integer_t x,
                                       eval_integer_t x_range) {
  eval_integer_t z;
  INTERPOLATE_(eval_log10_fetch, x, x_range, z);
  return z;
}

static eval_integer_t eval_exp_n10_10(eval_integer_t x,
                                      eval_integer_t x_range) {
  eval_integer_t z;
  INTERPOLATE_(eval_exp_fetch, x, x_range, z);
  return z;
}

static eval_integer_t eval_sqrt_lowp(eval_integer_t x,
                                     eval_integer_t scale) {
  eval_integer_t x_range = EVAL_SQRT_RANGE;
  eval_integer_t z;

  if (x < x_range) {
    z = eval_sqrt_0_10(x, x_range);
    z = eval_div(z, EVAL_SCALE, scale);
    return z;
  }

  eval_integer_t sqrt_10 = eval_sqrt_10(scale);
  eval_integer_t factor  = sqrt_10;
  x                      = eval_div(x, x_range, scale);

  while (x >= x_range) {
    factor = eval_mul(factor, sqrt_10, scale);
    x      = eval_div(x, x_range, scale);
  }

  z = eval_sqrt_0_10(x, x_range);
  z = eval_div(z, EVAL_SCALE, scale);
  z = eval_mul(z, factor, scale);

  return z;
}

eval_integer_t eval_sqrt(eval_integer_t x, eval_integer_t scale) {
  eval_integer_t upscale = eval_imul(scale, EVAL_UPSCALE);
  eval_integer_t z;
  z = eval_sqrt_lowp(eval_imul(x, EVAL_UPSCALE), upscale);
  z = eval_idiv(z, EVAL_UPSCALE);
  return z;
}

eval_integer_t eval_log(eval_integer_t x, eval_integer_t scale) {
  /*  FIXME
   */
  (void) eval_log_0_10;
  return 0;
}

eval_integer_t eval_log2(eval_integer_t x, eval_integer_t scale) {
  /*  FIXME
   */
  (void) eval_log2_0_10;
  return 0;
}

eval_integer_t eval_log10(eval_integer_t x, eval_integer_t scale) {
  /*  FIXME
   */
  (void) eval_log10_0_100;
  return 0;
}

eval_integer_t eval_exp(eval_integer_t x, eval_integer_t scale) {
  /*  FIXME
   */
  (void) eval_exp_n10_10;
  return 0;
}

eval_integer_t eval_sin(eval_integer_t x, eval_integer_t scale) {
  eval_integer_t x_range = EVAL_SIN_RANGE;
  eval_integer_t z;
  INTERPOLATE_(eval_sin_fetch_wrapped, x, x_range, z);
  return eval_div(z, EVAL_SCALE, scale);
}

eval_integer_t eval_tan(eval_integer_t x, eval_integer_t scale) {
  eval_integer_t x_range = EVAL_TAN_RANGE;
  eval_integer_t z;
  INTERPOLATE_(eval_tan_fetch_wrapped, x, x_range, z);
  return eval_div(z, EVAL_SCALE, scale);
}

eval_integer_t eval_asin(eval_integer_t x, eval_integer_t scale) {
  eval_integer_t x_range = EVAL_ASIN_RANGE;
  eval_integer_t z;
  INTERPOLATE_(eval_asin_fetch_sym, x, x_range, z);
  return eval_div(z, EVAL_SCALE, scale);
}

eval_integer_t eval_atan(eval_integer_t x, eval_integer_t scale) {
  eval_integer_t x_range = EVAL_ATAN_RANGE;
  eval_integer_t z;
  INTERPOLATE_(eval_atan_fetch_sym, x, x_range, z);
  return eval_div(z, EVAL_SCALE, scale);
}

/*  COMPOUND FUNCTIONS
 *
 *  Functions defined by compositions of basic functions and
 *  arithmetic operations.
 */

eval_integer_t eval_pow(eval_integer_t x, eval_integer_t y,
                        eval_integer_t scale) {
  //  x^y = (e^(ln x))^y = e^(y ln x)
  return eval_exp(eval_mul(eval_log(x, scale), y, scale), scale);
}

eval_integer_t eval_cos(eval_integer_t x, eval_integer_t scale) {
  //  cos(x) = sin(x + pi/2)
  return eval_sin(eval_add(x, eval_pi_2(scale)), scale);
}

eval_integer_t eval_acos(eval_integer_t x, eval_integer_t scale) {
  //  acos(x) = pi/2 + asin(-x)
  return eval_add(eval_pi_2(scale), eval_asin(eval_neg(x), scale));
}

eval_integer_t eval_atan2(eval_integer_t y, eval_integer_t x,
                          eval_integer_t scale) {
  if (x > 0)
    //  atan(y / x)
    return eval_atan(eval_div(y, x, scale), scale);
  else if (x < 0) {
    if (y >= 0)
      //  atan(y / x) + pi
      return eval_add(eval_atan(eval_div(y, x, scale), scale),
                      eval_pi(scale));
    else
      //  atan(y / x) - pi
      return eval_sub(eval_atan(eval_div(y, x, scale), scale),
                      eval_pi(scale));
  } else if (y > 0)
    //  pi / 2
    return eval_pi_2(scale);
  else if (y < 0)
    //  -pi / 2
    return -eval_pi_2(scale);
  //  undefined
  return 0;
}
