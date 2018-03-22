include_directories(${CTEST_INCLUDE_DIRS} ${CII_INCLUDE_DIRS})
add_executable(test_fe test_fe.c utils.c)
target_link_libraries(test_fe ${CII_LIBRARIES} ${FLEX_LIBRARIES} tparse)
