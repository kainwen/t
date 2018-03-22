cmake_minimum_required (VERSION 3.0)

project (t)

########## Set Shared Library Output Dir ###########
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")
set(RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

############### Set Include Dir ####################
include_directories ("${PROJECT_SOURCE_DIR}/include")

message("${RUNTIME_OUTPUT_DIRECTORY}")

############### check dependencies #################
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmakes")

find_package(FLEX REQUIRED)
find_package(CII REQUIRED)
find_package(Ctest REQUIRED)
find_package(LLVM REQUIRED)

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

###############################################################################
# Isolation tests
set(ISOLATION_TEST_ROOT "${PROJECT_SOURCE_DIR}/tests/isolation")
set(ISOLATION_TEST_DRIVER  "${ISOLATION_TEST_ROOT}/isolation.py")
set(COMPILER_BIN_PATH "${CMAKE_INSTALL_PREFIX}/bin/compile")

add_custom_target(isolation
                  COMMAND python3 "${ISOLATION_TEST_DRIVER}" "-d" "${ISOLATION_TEST_ROOT}" "-c" "${COMPILER_BIN_PATH}"
                  WORKING_DIRECTORY "${ISOLATION_TEST_ROOT}")
