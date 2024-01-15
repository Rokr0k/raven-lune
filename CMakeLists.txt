cmake_minimum_required(VERSION 3.10)

set(CMAKE_TOOLCHAIN_FILE
    "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")

project(raven-lune)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 CONFIG REQUIRED)
find_package(Stb REQUIRED)

add_subdirectory(libs/bmsparser)

add_executable(raven-lune "src/stb.cpp" "src/main.cpp")

target_compile_features(raven-lune PRIVATE cxx_std_17)

target_include_directories(raven-lune PRIVATE ${Stb_INCLUDE_DIR})

target_link_libraries(raven-lune PRIVATE bmsparser SDL2::SDL2 SDL2::SDL2main)

file(COPY "res/" DESTINATION "${CMAKE_BINARY_DIR}/res")
add_custom_target(
  raven-res ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/res
          $<TARGET_FILE_DIR:raven-lune>/res
  DEPENDS raven-lune)
