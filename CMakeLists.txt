cmake_minimum_required(VERSION 3.10)
project(GameTests)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(run_tests test.cpp)
target_link_libraries(run_tests gtest_main)