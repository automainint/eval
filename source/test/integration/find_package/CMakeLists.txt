cmake_minimum_required(VERSION 3.16)

project(eval_integration_find_package_root)

include(ExternalProject)

ExternalProject_Add(
  eval
  GIT_REPOSITORY  https://github.com/automainint/eval.git
  GIT_TAG         dev
  CMAKE_ARGS
  -D CMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_SOURCE_DIR}/install
  -D EVAL_ENABLE_TESTING=OFF)

ExternalProject_Add(
  eval_integration_find_package
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source
  CMAKE_ARGS
  -D CMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_SOURCE_DIR}/install)

ExternalProject_Add_StepDependencies(eval_integration_find_package build eval)

include(GNUInstallDirs)

enable_testing()

add_test(
  NAME eval_integration_find_package_run
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/install/${CMAKE_INSTALL_BINDIR}/eval_integration_find_package${CMAKE_EXECUTABLE_SUFFIX})

set_tests_properties(
  eval_integration_find_package_run
  PROPERTIES
  TIMEOUT "15")
