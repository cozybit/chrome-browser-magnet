cmake_minimum_required (VERSION 2.6) 

project(npmagnet)

if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")
endif()

find_library(PTHREAD REQUIRED NAMES pthread)

if (COMPILE_32BIT_NPAPI)

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
    set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS} -m32")

    set(MAGNET_LIB_HINT "./lib/32")

else()

    set(MAGNET_LIB_HINT "./lib/64")

endif()

find_library ( MAGNET   REQUIRED NAMES magnet   HINTS "${MAGNET_LIB_HINT}")
find_library ( ZMQ      REQUIRED NAMES zmq      HINTS "${MAGNET_LIB_HINT}")
find_library ( CZMQ     REQUIRED NAMES czmq     HINTS "${MAGNET_LIB_HINT}")

find_package(Threads REQUIRED)
include_directories(${Threads_INCLUDE_DIRS})

find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIRS})

include_directories(
	../magnet/core_include
	../magnet/agent_include
	../magnet/native_include
	)

add_executable (testclient testclient.cpp)

target_link_libraries(testclient ${ZMQ})
target_link_libraries(testclient ${CZMQ})
target_link_libraries(testclient ${MAGNET})
target_link_libraries(testclient ${Threads_LIBRARIES})
target_link_libraries(testclient ${ZLIB_LIBRARIES})
target_link_libraries(testclient ${PTHREAD})

add_library(npmagnet SHARED np_entry.cc npp_entry.cc plugin.cc)

target_link_libraries(npmagnet ${ZMQ})
target_link_libraries(npmagnet ${CZMQ})
target_link_libraries(npmagnet ${MAGNET})
target_link_libraries(npmagnet ${Threads_LIBRARIES})
target_link_libraries(npmagnet ${ZLIB_LIBRARIES})
target_link_libraries(npmagnet ${PTHREAD})
