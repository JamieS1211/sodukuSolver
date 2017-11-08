cmake_minimum_required(VERSION 3.8)
project(sudokuSolver)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c solveFunctions.c)
add_executable(sudokuSolver ${SOURCE_FILES})