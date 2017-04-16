set(HEADERS_LIST
    include/genui/markup/lexer.hpp
    include/genui/markup/object_model.hpp
    include/genui/markup/parser.hpp
    include/genui/markup/source_code.hpp
    include/genui/markup/token.hpp)

set(SRC_LIST
    src/lexer.cpp
    src/object_model.cpp
    src/parser.cpp
    src/source_code.cpp
    src/token.cpp)

source_group(include\\genui\\markup FILES ${HEADERS_LIST})
source_group(src FILES ${SRC_LIST})

add_library(genui.markup STATIC ${SRC_LIST} ${HEADERS_LIST})
target_include_directories(genui.markup PUBLIC include)

add_subdirectory(tests)