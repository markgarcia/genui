add_executable(genui.markup.tests.lexer_test main.cpp)
target_link_libraries(genui.markup.tests.lexer_test genui.markup)

add_test(
    NAME genui.markup.tests.lexer_test
    COMMAND genui.markup.tests.lexer_test)