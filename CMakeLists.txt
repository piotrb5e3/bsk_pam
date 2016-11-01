cmake_minimum_required(VERSION 3.6)
project(zad_pam)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

set(SOURCE_FILES main.c counter.c counter.h word_reader.c word_reader.h test.c test.c)
add_executable(zad_pam ${SOURCE_FILES})