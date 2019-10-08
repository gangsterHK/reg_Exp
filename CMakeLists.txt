cmake_minimum_required(VERSION 3.15)
project(reg_exp C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(reg_exp
        main.c
        regExpress.c
        regExpress.h)
