cmake_minimum_required (VERSION 3.0)

project (t)

########## Set Shared Library Output Dir ###########
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")

############### Set Include Dir ####################
include_directories ("${PROJECT_SOURCE_DIR}/include")

############### check dependencies #################
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmakes")

find_package(FLEX REQUIRED)
find_package(CII REQUIRED)
find_package(Ctest REQUIRED)

############### Set Compile Flags ##################
set(CMAKE_C_FLAGS_DEBUG "-g")

############### add subdirectories #################
add_subdirectory(src)
add_subdirectory(tests)

###############################################################################
# Unit tests
enable_testing()
set(UNIT_TEST_DATA_PATH "${PROJECT_SOURCE_DIR}/tests/data")
add_test(NAME test_fe COMMAND test_fe "${UNIT_TEST_DATA_PATH}")
