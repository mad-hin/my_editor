cmake_minimum_required(VERSION 3.15)
project(editor C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra")

add_executable(editor
        editor.c)

message(${CMAKE_CXX_FLAGS})