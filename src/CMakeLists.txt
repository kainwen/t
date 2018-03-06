FLEX_TARGET(MyLexer token.l ${CMAKE_CURRENT_BINARY_DIR}/lex.c)

include_directories(${CII_INCLUDE_DIRS})
add_library(tparse STATIC ast.c token.c ${FLEX_MyLexer_OUTPUTS})
target_link_libraries(tparse ${CII_LIBRARIES} ${FLEX_LIBRARIES})

install(TARGETS tparse ARCHIVE DESTINATION lib)
