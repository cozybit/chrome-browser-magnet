cmake_minimum_required (VERSION 2.6) 

project(npmagnet)

if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")
endif()

find_library ( ZMQ REQUIRED NAMES zmq )
find_library ( CZMQ REQUIRED NAMES czmq )

find_library(PTHREAD REQUIRED NAMES pthread)

find_library(MAGNET
	REQUIRED
	NAMES magnet
	HINTS "."
	)

find_package(Threads REQUIRED)
include_directories(${Threads_INCLUDE_DIRS})

find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIRS})

include_directories(
	../magnet/core_include
	../magnet/agent_include
	../magnet/native_include
	)

add_library(npmagnet SHARED np_entry.cc npp_entry.cc plugin.cc)

target_link_libraries(npmagnet ${ZMQ})
target_link_libraries(npmagnet ${CZMQ})
target_link_libraries(npmagnet ${MAGNET})
target_link_libraries(npmagnet ${Threads_LIBRARIES})
target_link_libraries(npmagnet ${ZLIB_LIBRARIES})
target_link_libraries(npmagnet ${PTHREAD})
