cmake_minimum_required(VERSION 3.10)

project(raven-lune)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

add_subdirectory(bmsparser)

set(CXX_SOURCES "src/file.cpp" "src/font.cpp" "src/audio.cpp" "src/image.cpp" "src/bga.cpp" "src/keys.cpp" "src/App.cpp" "src/TitleScene.cpp" "src/ListScene.cpp" "src/KeysScene.cpp" "src/PlayScene.cpp" "src/ResultScene.cpp" "src/main.cpp")
set(CXX_HEADERS "src/file.hpp" "src/font.hpp" "src/audio.hpp" "src/image.hpp" "src/bga.hpp" "src/keys.hpp" "src/App.hpp" "src/Scene.hpp" "src/TitleScene.hpp" "src/ListScene.hpp" "src/KeysScene.hpp" "src/PlayScene.hpp" "src/ResultScene.hpp")

add_executable(raven-lune ${CXX_SOURCES} ${CXX_HEADERS})

target_compile_features(raven-lune PRIVATE cxx_std_17)
target_link_libraries(raven-lune bmsparser SDL2::Main SDL2::Image SDL2::Mixer SDL2::TTF)

file(COPY "res/" DESTINATION "${CMAKE_BINARY_DIR}/res")