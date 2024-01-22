cmake_minimum_required(VERSION 3.10)

set(CMAKE_TOOLCHAIN_FILE
    "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")

project(raven-lune)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

find_package(Iconv REQUIRED)
find_package(ctre CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(Freetype REQUIRED)
find_package(FFMPEG REQUIRED)

add_subdirectory(libs/parser)
add_subdirectory(libs/core)
add_subdirectory(game)
