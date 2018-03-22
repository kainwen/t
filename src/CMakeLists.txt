FLEX_TARGET(MyLexer token.l ${CMAKE_CURRENT_BINARY_DIR}/lex.c)

include_directories(${CII_INCLUDE_DIRS} ${LLVM_INCLUDE_DIRS})
add_library(t STATIC ast.c token.c ${FLEX_MyLexer_OUTPUTS} codegen.c)
target_link_libraries(t ${CII_LIBRARIES} ${FLEX_LIBRARIES} ${LLVM_LIBRARIES})

add_library(tparse STATIC ast.c token.c ${FLEX_MyLexer_OUTPUTS})
target_link_libraries(t ${CII_LIBRARIES} ${FLEX_LIBRARIES})

include_directories(${LLVM_INCLUDE_DIRS})
add_executable(compile compile.c)
set_target_properties(compile
                      PROPERTIES COMPILE_FLAGS ${LLVM_CFLAGS}
                                 LINK_FLAGS ${LLVM_LDFLAGS})
target_link_libraries(compile t ${CII_LIBRARIES} ${FLEX_LIBRARIES} ${LLVM_LIBRARIES})
set_target_properties(compile PROPERTIES LINKER_LANGUAGE CXX)

install(TARGETS tparse ARCHIVE DESTINATION lib)
install(TARGETS t ARCHIVE DESTINATION lib)
install(TARGETS compile RUNTIME DESTINATION bin)

