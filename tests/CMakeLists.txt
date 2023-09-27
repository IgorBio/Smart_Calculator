cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Tests)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.1
)

FetchContent_MakeAvailable(googletest)

target_compile_options(gtest PRIVATE "-w")
target_compile_options(gmock PRIVATE "-w") 

include_directories(
  ${PROJECT_SOURCE_DIR}/../model/
)

add_executable(${PROJECT_NAME}
  ${PROJECT_SOURCE_DIR}/../model/math_calc.cc
  ${PROJECT_SOURCE_DIR}/../model/credit_calc.cc
  math_tests.cc
  credit_tests.cc
)

enable_testing()

target_compile_options(
    ${PROJECT_NAME}
    PUBLIC
    -Wall
    -Werror
    -Wextra
    -Wpedantic
    -std=c++17
)

target_link_libraries(${PROJECT_NAME} PUBLIC gtest gtest_main)
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
