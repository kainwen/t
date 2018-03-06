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

############### add subdirectories #################
add_subdirectory(src)
add_subdirectory(tests)

###############################################################################
# Unit tests
enable_testing()
add_test(NAME test_ COMMAND test_tok)
