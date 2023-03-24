target_sources(
  eval
    PRIVATE
      functions.c
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/arithmetic.h>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/functions.h>)
