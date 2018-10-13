cmake_minimum_required(VERSION 3.12)
project(project C)

set(CMAKE_C_STANDARD 99)

add_executable(project main.c ukkonen.c ukkonen.h)