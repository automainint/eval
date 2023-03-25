#include "functions.h"

#include "arithmetic.h"
#include "data.inl.h"

eval_integer_t eval_pi(eval_integer_t scale) {
  return eval_div(314159265358979324ll, //
                  100000000000000000ll, //
                  scale);
}

eval_integer_t eval_pi_2(eval_integer_t scale) {
  return eval_div(314159265358979324ll / 2, //
                  100000000000000000ll,     //
                  scale);
}

eval_integer_t eval_pi_4(eval_integer_t scale) {
  return eval_div(314159265358979324ll / 4, //
                  100000000000000000ll,     //
                  scale);
}

eval_integer_t eval_2_pi(eval_integer_t scale) {
  return eval_div(314159265358979324ll * 2, //
                  100000000000000000ll,     //
                  scale);
}

eval_integer_t eval_4_pi(eval_integer_t scale) {
  return eval_div(314159265358979324ll * 4, //
                  100000000000000000ll,     //
                  scale);
}

eval_integer_t eval_e(eval_integer_t scale) {
  return eval_div(271828182845904524ll, //
                  100000000000000000ll, //
                  scale);
}

eval_integer_t eval_e_2(eval_integer_t scale) {
  return eval_div(271828182845904524ll / 2, //
                  100000000000000000ll,     //
                  scale);
}

eval_integer_t eval_e_4(eval_integer_t scale) {
  return eval_div(271828182845904524ll / 4, //
                  100000000000000000ll,     //
                  scale);
}

eval_integer_t eval_2_e(eval_integer_t scale) {
  return eval_div(271828182845904524ll * 2, //
                  100000000000000000ll,     //
                  scale);
}

eval_integer_t eval_4_e(eval_integer_t scale) {
  return eval_div(271828182845904524ll * 4, //
                  100000000000000000ll,     //
                  scale);
}

/*  Basic functions.
 */

/*  TODO
 *  Smooth interpolation.
 */

eval_integer_t eval_sqrt(eval_integer_t x, eval_integer_t scale) {
  eval_integer_t const x_range = EVAL_SQRT_RANGE;

  eval_integer_t const n = eval_div(x, x_range, EVAL_RANGE);

  if (n < 0)
    return 0;
  if (n > EVAL_RANGE)
    return EVAL_MAX;

  eval_integer_t const z = eval_data_sqrt[n];
  return eval_div(z, scale, EVAL_SCALE);
}

eval_integer_t eval_log(eval_integer_t x, eval_integer_t scale) {
  eval_integer_t const x_range = EVAL_LOG_RANGE;

  eval_integer_t const n = eval_div(x, x_range, EVAL_RANGE);

  if (n <= 0)
    return EVAL_MIN;
  if (n > EVAL_RANGE)
    return EVAL_MAX;

  eval_integer_t const z = eval_data_log[n - 1];
  return eval_div(z, scale, EVAL_SCALE);
}

eval_integer_t eval_exp(eval_integer_t x, eval_integer_t scale) {
  eval_integer_t const x_min   = EVAL_EXP_MIN;
  eval_integer_t const x_range = EVAL_EXP_RANGE;

  eval_integer_t const n = eval_div(eval_sub(x, x_min), x_range,
                                    EVAL_RANGE);

  if (n < 0)
    return 0;
  if (n > EVAL_RANGE)
    return EVAL_MAX;

  eval_integer_t const z = eval_data_exp[n];
  return eval_div(z, scale, EVAL_SCALE);
}

eval_integer_t eval_sin(eval_integer_t x, eval_integer_t scale) {
  eval_integer_t const x_range = EVAL_SIN_RANGE;

  eval_integer_t n = eval_div(x, x_range, EVAL_RANGE);
  n                = eval_wrap(n, 0, EVAL_RANGE);

  eval_integer_t const z = eval_data_sin[n];
  return eval_div(z, scale, EVAL_SCALE);
}

eval_integer_t eval_tan(eval_integer_t x, eval_integer_t scale) {
  eval_integer_t const x_range = EVAL_TAN_RANGE;

  eval_integer_t n = eval_div(x, x_range, EVAL_RANGE);
  n                = eval_wrap(n, -EVAL_RANGE, EVAL_RANGE * 2);

  if (n <= -EVAL_RANGE)
    //  +/- infinity
    return 0;

  eval_integer_t const z = n < 0 ? -eval_data_tan[-n]
                                 : eval_data_tan[n];
  return eval_div(z, scale, EVAL_SCALE);
}

eval_integer_t eval_asin(eval_integer_t x, eval_integer_t scale) {
  eval_integer_t const x_range = EVAL_ASIN_RANGE;

  eval_integer_t n = eval_div(x, x_range, EVAL_RANGE);
  n                = eval_wrap(n, -EVAL_RANGE, EVAL_RANGE * 2);

  if (n < -EVAL_RANGE)
    return -eval_pi_2(scale);
  if (n > EVAL_RANGE)
    return eval_pi_2(scale);

  eval_integer_t const z = n < 0 ? -eval_data_asin[-n]
                                 : eval_data_asin[n];

  return eval_div(z, scale, EVAL_SCALE);
}

eval_integer_t eval_atan(eval_integer_t x, eval_integer_t scale) {
  eval_integer_t const x_range = EVAL_ATAN_RANGE;

  eval_integer_t n = eval_div(x, x_range, EVAL_RANGE);
  n                = eval_wrap(n, -EVAL_RANGE, EVAL_RANGE * 2);

  if (n < -EVAL_RANGE)
    return -eval_pi_2(scale);
  if (n > EVAL_RANGE)
    return eval_pi_2(scale);

  eval_integer_t const z = n < 0 ? -eval_data_atan[-n]
                                 : eval_data_atan[n];

  return eval_div(z, scale, EVAL_SCALE);
}

/*  Compound functions.
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
