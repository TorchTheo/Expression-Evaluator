cmake_minimum_required(VERSION 3.19)
project(ExpCalc)

set(CMAKE_CXX_STANDARD 14)

add_executable(ExpCalc main.cpp Calc.cpp Calc.h Variable.cpp Variable.h Calculator.cpp Calculator.h Func.cpp Func.h)