cmake_minimum_required(VERSION 3.10)

project(raven-lune)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
pkg_check_modules(LIBVLC REQUIRED libvlc)

add_subdirectory("bmsparser")

set(CXX_SOURCES "src/file.cpp" "src/font.cpp" "src/audio.cpp" "src/image.cpp" "src/video.cpp" "src/bga.cpp" "src/keys.cpp" "src/App.cpp" "src/TitleScene.cpp" "src/ListScene.cpp" "src/KeysScene.cpp" "src/PlayScene.cpp" "src/ResultScene.cpp" "src/main.cpp")
set(CXX_HEADERS "src/file.hpp" "src/font.hpp" "src/audio.hpp" "src/image.hpp" "src/video.hpp" "src/bga.hpp" "src/keys.hpp" "src/App.hpp" "src/Scene.hpp" "src/TitleScene.hpp" "src/ListScene.hpp" "src/KeysScene.hpp" "src/PlayScene.hpp" "src/ResultScene.hpp")

add_executable(raven-lune ${CXX_SOURCES} ${CXX_HEADERS})

target_compile_features(raven-lune PRIVATE cxx_std_17)

target_include_directories(raven-lune PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${LIBVLC_INCLUDE_DIRS})
target_link_libraries(raven-lune PRIVATE bmsparser ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_MIXER_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${LIBVLC_LIBRARIES})

file(COPY "res/" DESTINATION "${CMAKE_BINARY_DIR}/res")