cmake_minimum_required(VERSION 3.16)
set(NAME eval-integration-fetch-content)
project(${NAME} C)

include(FetchContent)
FetchContent_Declare(
  eval
  GIT_REPOSITORY https://github.com/automainint/eval.git
  GIT_TAG dev)
set(EVAL_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(eval)

add_executable(${NAME})
target_sources(${NAME} PRIVATE main.c)
target_link_libraries(${NAME} PRIVATE eval::eval)

enable_testing()
add_test(NAME ${NAME}-run COMMAND ${NAME})
set_tests_properties(${NAME}-run PROPERTIES TIMEOUT "15")
