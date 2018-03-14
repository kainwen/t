include_directories(${CTEST_INCLUDE_DIRS} ${CII_INCLUDE_DIRS})
add_executable(test_fe test_fe.c utils.c)
target_link_libraries(test_fe ${CII_LIBRARIES} ${FLEX_LIBRARIES} tparse)

include_directories(${LLVM_INCLUDE_DIRS})
add_executable(test_codegen test_codegen.c utils.c)
set_target_properties(test_codegen
                      PROPERTIES COMPILE_FLAGS ${LLVM_CFLAGS}
                                 LINK_FLAGS ${LLVM_LDFLAGS})
target_link_libraries(test_codegen
                      ${CII_LIBRARIES} ${FLEX_LIBRARIES}
                      ${LLVM_LIBRARIES}
                      t)
set_target_properties(test_codegen PROPERTIES LINKER_LANGUAGE CXX)
