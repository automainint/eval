target_sources(
  ${EVAL_LIBRARY}
    PRIVATE
      eval.c
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/eval.h>)
