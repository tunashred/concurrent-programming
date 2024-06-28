cmake_minimum_required(VERSION 3.0.0)
project(concurrent-programming VERSION 0.1.0 LANGUAGES C)

add_compile_options(-Wall -Wextra)

set(SOURCE_FILE processes/test.c)
add_executable(concurrent-programming ${SOURCE_FILE})


