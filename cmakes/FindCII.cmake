find_library(CII_LIBRARY NAMES cii
  PATHS /usr/lib /usr/local/lib
  DOC "Path to the cii library")

find_path(CII_INCLUDE_DIR cii
  PATHS /usr/include /usr/local/include
  DOC "Path to the cii headers")

set(CII_INCLUDE_DIRS ${CII_INCLUDE_DIR})
set(CII_LIBRARIES ${CII_LIBRARY})

message("-- Found CII at ${CII_INCLUDE_DIR}.")