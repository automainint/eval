cmake_minimum_required(VERSION 3.16)

project(eval_integration_find_package C)

find_package(eval REQUIRED)

add_executable(eval_integration_find_package)
target_sources(eval_integration_find_package PRIVATE main.c)
target_link_libraries(eval_integration_find_package PRIVATE eval::eval)
install(TARGETS eval_integration_find_package RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
