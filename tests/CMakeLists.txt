include_directories(${CTEST_INCLUDE_DIRS} ${CII_INCLUDE_DIRS})
add_executable(test_tok test.c test_token.c)
target_link_libraries(test_tok ${CII_LIBRARIES} ${FLEX_LIBRARIES} tparse)
