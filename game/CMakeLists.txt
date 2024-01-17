add_executable(
  rl
  "src/main.cpp"
  "src/stb.cpp"
  "src/utils.cpp"
  "src/app.cpp"
  "src/scene.cpp"
  "src/audio.cpp"
  "src/font.cpp"
  "src/title.cpp")

target_include_directories(rl PRIVATE ${Stb_INCLUDE_DIR} ${FFMPEG_INCLUDE_DIRS})

target_link_directories(rl PRIVATE ${FFMPEG_LIBRARY_DIRS})

target_link_libraries(rl PRIVATE parser SDL2::SDL2 SDL2::SDL2main Freetype::Freetype Iconv::Iconv
                                 ${FFMPEG_LIBRARIES})

add_custom_target(
  rlres ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/res
          $<TARGET_FILE_DIR:rl>/res
  DEPENDS rl)
