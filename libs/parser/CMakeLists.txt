add_library(parser STATIC "src/parser.cpp" "src/convert.cpp")

target_include_directories(parser PUBLIC "include/")

target_link_libraries(parser PRIVATE Iconv::Iconv)
