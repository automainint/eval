#include "../../eval/arithmetic.h"

#define KIT_TEST_FILE arithmetic
#include <kit_test/test.h>

static int64_t const max_ = INT64_MAX;

TEST("arithmetic min and max") {
  REQUIRE(EVAL_MIN == -EVAL_MAX);
  REQUIRE(EVAL_MAX == -EVAL_MIN);
}

TEST("arithmetic neg") {

  REQUIRE(eval_neg(0) == 0);
  REQUIRE(eval_neg(4) == -4);
  REQUIRE(eval_neg(-4) == 4);
  REQUIRE(eval_neg(max_) + max_ == 0);
  REQUIRE(eval_neg(eval_neg(max_)) + eval_neg(max_) == 0);
}

TEST("arithmetic add") {
  REQUIRE(eval_add(1, 2) == 3);
  REQUIRE(eval_add(4, 5) == 9);
  REQUIRE(eval_add(1, -2) == -1);
  REQUIRE(eval_add(5, -4) == 1);
  REQUIRE(eval_add(max_, 1) == EVAL_MAX);
  REQUIRE(eval_add(-max_, -1) == EVAL_MIN);
  REQUIRE(eval_add(max_, max_) == EVAL_MAX);
  REQUIRE(eval_add(-max_, -max_) == EVAL_MIN);
  REQUIRE(eval_add(max_ / 2, 1 + max_ / 2) == EVAL_MAX);
  REQUIRE(eval_add(-max_ / 2, -1 - max_ / 2) == EVAL_MIN);
}

TEST("arithmetic sub") {
  REQUIRE(eval_sub(1, 2) == -1);
  REQUIRE(eval_sub(5, 4) == 1);
  REQUIRE(eval_sub(1, -2) == 3);
  REQUIRE(eval_sub(5, -4) == 9);
  REQUIRE(eval_sub(max_, -1) == EVAL_MAX);
  REQUIRE(eval_sub(-max_, 1) == EVAL_MIN);
  REQUIRE(eval_sub(max_, -max_) == EVAL_MAX);
  REQUIRE(eval_sub(-max_, max_) == EVAL_MIN);
  REQUIRE(eval_sub(max_ / 2, -1 - max_ / 2) == EVAL_MAX);
  REQUIRE(eval_sub(-max_ / 2, 1 + max_ / 2) == EVAL_MIN);
}

TEST("arithmetic wrap") {
  REQUIRE(eval_wrap(5, 5, 10) == 5);
  REQUIRE(eval_wrap(14, 5, 10) == 14);
  REQUIRE(eval_wrap(15, 5, 10) == 5);
  REQUIRE(eval_wrap(4, 5, 10) == 14);
  REQUIRE(eval_wrap(-5, 5, 10) == 5);
  REQUIRE(eval_wrap(-6, 5, 10) == 14);
}

/*  FIXME
 *  Add more tests.
 */
