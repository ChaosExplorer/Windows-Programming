CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(SOAPnuke)

ADD_DEFINITIONS(-Wall -O2)#附加编译选项 


#LINK_DIRECTORIES(/usr/local/lib/)

find_library(temp1 libboost_system.a PATHS ENV LD_LIBRARY_PATH)
message(STATUS "boost_system: ${temp1}")
add_library(system STATIC IMPORTED)
set_property(TARGET system PROPERTY IMPORTED_LOCATION
		${temp1})

find_library(temp2 libboost_thread.a PATHS ENV LD_LIBRARY_PATH)
message(STATUS "boost_thread: ${temp2}")
add_library(thread STATIC IMPORTED)
set_property(TARGET thread PROPERTY IMPORTED_LOCATION
		${temp2})
find_library(temp3 libcrypto.a PATHS ENV LD_LIBRARY_PATH)
message(STATUS "crtpto: ${temp3}")
add_library(openssl STATIC IMPORTED)
set_property(TARGET openssl PROPERTY IMPORTED_LOCATION
		${temp3})
find_library(temp4 libz.a PATHS ENV LD_LIBRARY_PATH)
message(STATUS "zlib: ${temp4}")
add_library(zlib STATIC IMPORTED)
set_property(TARGET zlib PROPERTY IMPORTED_LOCATION
		${temp4})
find_library(temp5 liblog4cplus.a PATHS ENV LD_LIBRARY_PATH)
message(STATUS "log4cplus: ${temp5}")
add_library(log4cplus STATIC IMPORTED)
set_property(TARGET log4cplus PROPERTY IMPORTED_LOCATION
		${temp5})


ADD_EXECUTABLE(SOAPnuke Main.cpp Common.cpp FilterProcessor.cpp FqBuffer.cpp
        	PeBuffer.cpp Logger.cpp SRNACommon.cpp SRNAProcessor.cpp
		SRNACleanFA.cpp
		SRNACleanFQ.cpp DGECommon.cpp DGEProcessor.cpp FqFile.cpp
		MetaCommon.cpp MetaProcessor.cpp)

TARGET_LINK_LIBRARIES(SOAPnuke system openssl thread log4cplus zlib pthread)


set(CMAKE_C_FLAGS " -O2 -g -pg")
set(CMAKE_CXX_FLAGS "-O2 -g -pg")
set(CMAKE_LEGACY_CYGWIN_WIN32 0)
set(CMAKE_CXX_STANDARD 11)





