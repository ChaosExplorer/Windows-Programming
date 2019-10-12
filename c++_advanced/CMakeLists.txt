cmake_minimum_required(VERSION 3.12)
project(c___advanced)

set(CMAKE_CXX_STANDARD 14)

aux_source_directory(. SRC)
aux_source_directory(Method M_SRC)

add_library(c___advanced SHARED ${SRC} ${M_SRC})