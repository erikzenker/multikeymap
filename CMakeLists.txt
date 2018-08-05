cmake_minimum_required(VERSION 3.5)
project(multikeymap)

include_directories("include/")

set(CMAKE_CXX_FLAGS_DEBUG
  "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Wpedantic -g3 -O0"
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

add_subdirectory(example)
add_subdirectory(test)