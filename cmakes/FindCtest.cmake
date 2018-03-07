find_path(CTEST_INCLUDE_DIR ctest.h
  PATHS /usr/include /usr/local/include
  DOC "Path to the ctest headers")

message(STATUS "Found Ctest include path: ${CTEST_INCLUDE_DIR}")

set(CTEST_INCLUDE_DIRS ${CTEST_INCLUDE_DIR})
