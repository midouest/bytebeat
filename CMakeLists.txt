cmake_minimum_required(VERSION 3.19)
project(bytebeat)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(bytebeat src/main.cpp
                        src/parse.cpp
                        src/tokenize.cpp)
target_include_directories(bytebeat PRIVATE include)

find_package(Catch2 REQUIRED)
add_executable(test_bytebeat test/test.cpp
                             src/parse.cpp
                             src/tokenize.cpp
                             test/expression_test.cpp
                             test/parse_test.cpp
                             test/tokenize_test.cpp)
target_include_directories(test_bytebeat PRIVATE include)
target_link_libraries(test_bytebeat PRIVATE Catch2::Catch2)
