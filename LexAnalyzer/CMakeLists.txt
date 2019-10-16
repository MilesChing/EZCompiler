add_library(lex_analyzer lex_analyzer.cpp lex_component.cpp)

add_executable(ezlex ezlex.cpp)
target_link_libraries(ezlex lex_analyzer highlighter)