cmake_minimum_required(VERSION 3.10)

project(raven-lune)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_subdirectory("parser/")

set(CXX_SOURCES "src/main.cpp" "src/App.cpp" "src/TitleScene.cpp" "src/ListScene.cpp" "src/font.cpp")
set(CXX_HEADERS "src/App.hpp" "src/Scene.hpp" "src/TitleScene.hpp" "src/ListScene.hpp" "src/font.hpp")

add_executable(raven-lune ${CXX_SOURCES} ${CXX_HEADERS})

target_compile_features(raven-lune PRIVATE cxx_std_17)
target_link_libraries(raven-lune SDL2::Main SDL2::Image SDL2::TTF bmsparser)