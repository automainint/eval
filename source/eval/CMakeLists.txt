target_sources(
  eval
    PRIVATE
      eval.c
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/eval.h>)
