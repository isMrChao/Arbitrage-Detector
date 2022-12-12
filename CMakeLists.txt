cmake_minimum_required(VERSION 3.21)
project(Final_Project)

set(CMAKE_CXX_STANDARD 20)

add_executable(Final_Project main.cpp arbitrage.cpp arbitrage.h)

add_executable(Test_Final_Project test_main.cpp tests.cpp arbitrage.cpp arbitrage.h)
