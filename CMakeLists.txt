cmake_minimum_required(VERSION 3.21)
project(Final_Project)

set(CMAKE_CXX_STANDARD 20)

add_executable(Final_Project main.cpp arbitrage.cpp arbitrage.h stdc++.h)

Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.0.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)

add_executable(Test_Final_Project tests.cpp arbitrage.cpp arbitrage.h stdc++.h)
target_link_libraries(Test_Final_Project PRIVATE Catch2::Catch2WithMain)