#ifndef EVAL_FUNCTIONS_H
#define EVAL_FUNCTIONS_H

#include "options.h"

#ifdef __cplusplus
extern "C" {
#endif

eval_integer_t eval_pi(eval_integer_t scale);
eval_integer_t eval_pi_2(eval_integer_t scale);
eval_integer_t eval_pi_4(eval_integer_t scale);
eval_integer_t eval_2_pi(eval_integer_t scale);
eval_integer_t eval_4_pi(eval_integer_t scale);
eval_integer_t eval_e(eval_integer_t scale);
eval_integer_t eval_e_2(eval_integer_t scale);
eval_integer_t eval_e_4(eval_integer_t scale);
eval_integer_t eval_2_e(eval_integer_t scale);
eval_integer_t eval_4_e(eval_integer_t scale);

eval_integer_t eval_sqrt(eval_integer_t x, eval_integer_t scale);
eval_integer_t eval_log(eval_integer_t x, eval_integer_t scale);
eval_integer_t eval_exp(eval_integer_t x, eval_integer_t scale);
eval_integer_t eval_sin(eval_integer_t x, eval_integer_t scale);
eval_integer_t eval_tan(eval_integer_t x, eval_integer_t scale);
eval_integer_t eval_asin(eval_integer_t x, eval_integer_t scale);
eval_integer_t eval_atan(eval_integer_t x, eval_integer_t scale);
eval_integer_t eval_pow(eval_integer_t x, eval_integer_t y,
                        eval_integer_t scale);
eval_integer_t eval_cos(eval_integer_t x, eval_integer_t scale);
eval_integer_t eval_acos(eval_integer_t x, eval_integer_t scale);
eval_integer_t eval_atan2(eval_integer_t y, eval_integer_t x,
                          eval_integer_t scale);

#ifdef __cplusplus
}
#endif

#endif
