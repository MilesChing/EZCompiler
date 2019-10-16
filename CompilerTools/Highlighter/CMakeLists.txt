add_library(colors colors.cpp)
add_library(highlighter highlighter.cpp)

target_link_libraries(highlighter colors)