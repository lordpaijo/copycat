cmake_minimum_required(VERSION 3.10)

project(copycat C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(copycat
    src/main.c
)

install(TARGETS copycat
    RUNTIME DESTINATION bin
)
