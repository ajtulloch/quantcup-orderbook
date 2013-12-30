cmake_minimum_required (VERSION 2.6) 
project (ORDERBOOK)

find_package(Boost 1.54 COMPONENTS REQUIRED)
include_directories( ${Boost_INCLUDE_DIR} )

# Initialize CXXFLAGS.
set(CMAKE_CXX_FLAGS "-Wall -Wno-missing-braces -std=c++11")
set(CMAKE_BUILD_TYPE Release)

set(TEST_SRCS order_book.cpp engine.cpp test.cpp)
set(SCORE_SRCS order_book.cpp engine.cpp score.cpp)

add_executable(test ${TEST_SRCS})
add_executable(score ${SCORE_SRCS})