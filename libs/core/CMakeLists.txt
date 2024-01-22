add_library(
  core STATIC
  "src/utils.cpp"
  "src/app.cpp"
  "src/scene.cpp"
  "src/image.cpp"
  "src/audio.cpp"
  "src/video.cpp"
  "src/font.cpp"
  "src/stb.cpp")

target_include_directories(core PUBLIC "include/")
target_link_libraries(
  core PUBLIC $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>)

target_include_directories(core PRIVATE ${Stb_INCLUDE_DIR}
                                        ${FFMPEG_INCLUDE_DIRS})
target_link_directories(core PRIVATE ${FFMPEG_LIBRARY_DIRS})
target_link_libraries(core PRIVATE Freetype::Freetype Iconv::Iconv
                                   ${FFMPEG_LIBRARIES})
