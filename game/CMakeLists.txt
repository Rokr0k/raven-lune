add_executable(rl "src/main.cpp" "src/scenes/title.cpp" "src/scenes/list.cpp")

target_link_libraries(rl PRIVATE parser core
                                 $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>)

add_custom_target(
  rlres ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/res
          $<TARGET_FILE_DIR:rl>/res
  DEPENDS rl)
