cmake_minimum_required(VERSION 3.1.0)
project(samhandler)

add_subdirectory(core)

set(CMAKE_CXX_STANDARD 11)

include_directories(core)

link_directories(core)

aux_source_directory(. ROOT_DIR_SRCS)

add_executable(samhandler ${ROOT_DIR_SRCS})
target_link_libraries(samhandler samtoolcore)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg -lc -pthread")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg -lc")