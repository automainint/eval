cmake_minimum_required(VERSION 3.16)

option(EVAL_ENABLE_TESTING "Enable testing" ON)

set(EVAL_PROJECT eval)
set(EVAL_LIBRARY eval)
set(EVAL_TEST eval_test)
set(EVAL_TARGETS eval-targets)

if(NOT DEFINED CMAKE_BUILD_PARALLEL_LEVEL)
  set(CMAKE_BUILD_PARALLEL_LEVEL 4)
endif()

project(
  ${EVAL_PROJECT}
  VERSION 0.1.1
  DESCRIPTION "Eval"
  LANGUAGES C)

include(FetchContent)

find_package(kit QUIET)

if(NOT kit_FOUND)
  FetchContent_Declare(
    kit
    GIT_REPOSITORY https://github.com/automainint/kit.git
    GIT_TAG dev)
  set(KIT_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(kit)
endif()

add_library(${EVAL_LIBRARY} STATIC)
add_library(${EVAL_PROJECT}::${EVAL_LIBRARY} ALIAS ${EVAL_LIBRARY})
target_include_directories(
  ${EVAL_LIBRARY} INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/source>)
target_compile_features(${EVAL_LIBRARY} PUBLIC c_std_11)
target_link_libraries(${EVAL_LIBRARY} PUBLIC kit::kit)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(${EVAL_LIBRARY} PUBLIC -pedantic -Wall -Werror)
endif()

enable_testing()

if(EVAL_ENABLE_TESTING)
  find_package(Threads REQUIRED)

  add_executable(${EVAL_TEST})
  add_executable(${EVAL_PROJECT}::${EVAL_TEST} ALIAS ${EVAL_TEST})
  target_compile_features(${EVAL_TEST} PRIVATE c_std_11)
  target_link_libraries(${EVAL_TEST} PRIVATE ${EVAL_LIBRARY} kit::kit_test)

  if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(
      ${EVAL_LIBRARY} PUBLIC
      -fsanitize=undefined,address
      --coverage -O0 -g)
    target_link_options(
      ${EVAL_LIBRARY} PUBLIC
      -fsanitize=undefined,address
      --coverage)
  endif()

  add_test(
    NAME ${EVAL_LIBRARY}_unittests
    COMMAND ${EVAL_TEST} --no-carriage-return)

  set_tests_properties(
    ${EVAL_LIBRARY}_unittests
    PROPERTIES
    TIMEOUT "120")
endif()

add_subdirectory(source)

include(GNUInstallDirs)

file(
  GLOB_RECURSE headers_
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/source/eval
  ${CMAKE_CURRENT_SOURCE_DIR}/source/eval/*.h)

foreach(path_ ${headers_})
  get_filename_component(dir_ "${path_}" DIRECTORY)

  install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/source/eval/${path_}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/eval/${dir_})
endforeach()

unset(headers_)
unset(path_)
unset(dir_)

install(
  TARGETS ${EVAL_LIBRARY} ${EVAL_TEST_LIBRARY}
  EXPORT ${EVAL_TARGETS}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(
  EXPORT ${EVAL_TARGETS}
  FILE ${EVAL_TARGETS}.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

export(
  EXPORT ${EVAL_TARGETS}
  FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/${EVAL_TARGETS}.cmake"
  NAMESPACE ${PROJECT_NAME}::)

include(CMakePackageConfigHelpers)

string(TOLOWER ${PROJECT_NAME} project_lower_)

configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/${project_lower_}-config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${project_lower_}-config-version.cmake"
  VERSION "${PROJECT_VERSION}"
  COMPATIBILITY AnyNewerVersion)

install(
  FILES
  "${CMAKE_CURRENT_BINARY_DIR}/${project_lower_}-config.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/${project_lower_}-config-version.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

unset(project_lower_)
