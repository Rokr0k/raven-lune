cmake_minimum_required(VERSION 3.10)

project(raven-lune)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(LIBVLC REQUIRED)
find_package(OpenGL)

add_subdirectory("bmsparser")

set(CXX_SOURCES "src/file.cpp" "src/font.cpp" "src/audio.cpp" "src/image.cpp" "src/video.cpp" "src/bga.cpp" "src/keys.cpp" "src/App.cpp" "src/TitleScene.cpp" "src/ListScene.cpp" "src/KeysScene.cpp" "src/PlayScene.cpp" "src/ResultScene.cpp" "src/main.cpp")
set(CXX_HEADERS "src/file.hpp" "src/font.hpp" "src/audio.hpp" "src/image.hpp" "src/video.hpp" "src/bga.hpp" "src/keys.hpp" "src/App.hpp" "src/Scene.hpp" "src/TitleScene.hpp" "src/ListScene.hpp" "src/KeysScene.hpp" "src/PlayScene.hpp" "src/ResultScene.hpp")

add_executable(raven-lune ${CXX_SOURCES} ${CXX_HEADERS})

target_compile_features(raven-lune PRIVATE cxx_std_17)

target_include_directories(raven-lune PRIVATE ${LIBVLC_INCLUDE_DIR})
target_link_libraries(raven-lune PRIVATE bmsparser SDL2::SDL2main SDL2::SDL2 SDL2_image::SDL2_image SDL2_mixer::SDL2_mixer SDL2_ttf::SDL2_ttf ${LIBVLC_LIBRARY} OpenGL::GL)

file(COPY "res/" DESTINATION "${CMAKE_BINARY_DIR}/res")
