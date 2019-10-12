cmake_minimum_required(VERSION 3.12)
project(segv_test)

set(CMAKE_CXX_STANDARD 14)

#link_libraries(/home/chaos/projects/bgi_gpu_server/lib/libbreakpad_client.a)
link_directories(/home/chaos/projects/bgi_gpu_server/lib)
include_directories(/home/chaos/projects/bgi_gpu_server/include/breakpad)

set(CMAKE_CXX_FLAGS  ${CMAKE_CXX_FLAGS} -g)

add_executable(segv_test main.cpp)

target_link_libraries(segv_test breakpad_client)
target_link_libraries(segv_test pthread)