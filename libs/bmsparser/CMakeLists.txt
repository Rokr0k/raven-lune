cmake_minimum_required(VERSION 3.10)

project(bmsparser)

add_library(bmsparser STATIC "src/bmsparser.cpp" "src/convert.cpp" "src/table.hpp")

target_include_directories(bmsparser PUBLIC "include/")
