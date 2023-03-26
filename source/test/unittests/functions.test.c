#include "../../eval/functions.h"

#define KIT_TEST_FILE functions
#include <kit_test/test.h>

TEST("functions sqrt") {
  REQUIRE(eval_sqrt(4, 1) == 2);
  REQUIRE(eval_sqrt(400, 100) == 200);
  REQUIRE(eval_sqrt(40000, 10000) == 20000);
  REQUIRE(eval_sqrt(4000000, 1000000) == 2000000);
  REQUIRE(eval_sqrt(100000, 1000) == 10000);
  REQUIRE(eval_sqrt(400000, 1000) == 20000);
}
