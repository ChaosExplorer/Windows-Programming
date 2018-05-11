cmake_minimum_required(VERSION 3.5)
project(fastq_gz_handler)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp FastqHandler.cpp FastqHandler.h)

set(LIB_PATH lib/zlib)

aux_source_directory(. ROOT_DIR_SRCS)

include_directories(${LIB_PATH})

link_directories(lib/zlib)

add_executable(fastq_gz_handler ${SOURCE_FILES})

target_link_libraries(fastq_gz_handler z)

set(CMAKE_BUILD_TYPE Debug)

#set(CMAKE_VERBOSE_MAKEFILE on)

#message(${PROJECT_BINARY_DIR})

#add_subdirectory(lib/zlib)

#add_definitions(-DFQSWITCH)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -pg -pthread")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg -lc")
set(CMAKE_SHARED_LINKER_FLAGS "{CMAKE_SHARED_LINKER_FLAGS} -pg -lc")
set(CMAKE_STATIC_LINKER_FLAGS  "{CMAKE_STATIC_LINKER_FLAGS} -pg -lc")
