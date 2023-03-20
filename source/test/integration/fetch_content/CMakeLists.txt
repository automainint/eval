cmake_minimum_required(VERSION 3.16)

project(eval_integration_fetch_content C)

include(FetchContent)
FetchContent_Declare(
  eval
  GIT_REPOSITORY https://github.com/automainint/eval.git
  GIT_TAG dev)
set(EVAL_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(eval)

add_executable(eval_integration_fetch_content)
target_sources(eval_integration_fetch_content PRIVATE main.c)
target_link_libraries(eval_integration_fetch_content PRIVATE eval::eval)

enable_testing()
add_test(NAME eval_integration_fetch_content_run COMMAND eval_integration_fetch_content)
set_tests_properties(eval_integration_fetch_content_run PROPERTIES TIMEOUT "15")
