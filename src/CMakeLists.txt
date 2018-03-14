FLEX_TARGET(MyLexer token.l ${CMAKE_CURRENT_BINARY_DIR}/lex.c)

include_directories(${CII_INCLUDE_DIRS} ${LLVM_INCLUDE_DIRS})
add_library(t STATIC ast.c token.c ${FLEX_MyLexer_OUTPUTS} codegen.c)
target_link_libraries(t ${CII_LIBRARIES} ${FLEX_LIBRARIES} ${LLVM_LIBRARIES})

add_library(tparse STATIC ast.c token.c ${FLEX_MyLexer_OUTPUTS})
target_link_libraries(t ${CII_LIBRARIES} ${FLEX_LIBRARIES})

install(TARGETS tparse ARCHIVE DESTINATION lib)
install(TARGETS t ARCHIVE DESTINATION lib)
