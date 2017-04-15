add_executable(genui.markup.tests.compilation_test main.cpp)
target_link_libraries(genui.markup.tests.compilation_test genui.markup)

add_test(
    NAME genui.markup.tests.compilation_test
    COMMAND genui.markup.tests.compilation_test)