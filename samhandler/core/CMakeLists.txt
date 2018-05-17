
project(samtool_core)

set(CMAKE_CXX_STANDARD 11)

aux_source_directory(. ALL_DIR_SRCS)

add_library(samtoolcore ${ALL_DIR_SRCS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg -pthread")
#set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -pg")