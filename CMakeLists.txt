# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.11)

message(STATUS "cmake home dir: [${CMAKE_HOME_DIRECTORY}].")
message(STATUS "cmake binary dir: [${CMAKE_BINARY_DIR}].")
message(STATUS "cmake source dir: [${CMAKE_SOURCE_DIR}].")
message(STATUS "cmake install profix dir: [${CMAKE_INSTALL_PREFIX}].")

project(cpp_uws_websocket_server)

set(CMAKE_CXX_STANDARD 14)

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)
set(CMAKE_CXX_FLAGS "-std=c++11")


find_package(Boost COMPONENTS filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})
link_directories(${OPENSSL_LIBRARY_DIR})

find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIR})
link_directories(${ZLIB_LIBRARY_DIR})

find_package(Threads REQUIRED)

add_library(uWS SHARED IMPORTED) # or STATIC instead of SHARED

set_target_properties(uWS PROPERTIES
        IMPORTED_LOCATION "${CMAKE_HOME_DIRECTORY}/uWebSockets/build/libuWS.dylib"
        )

find_library(UV_LIBRARY
        NAMES uv
        )
message(STATUS "UV dir: [${UV_LIBRARY}].")

add_dependencies(uWS uWS_p)

set(SAMP cpp-uws-websocket-server)
add_executable(${SAMP} ${CMAKE_HOME_DIRECTORY}/main.cpp server.cpp server.h)
target_link_libraries( ${SAMP} uWS )
target_link_libraries( ${SAMP}  ${UV_LIBRARY})
target_link_libraries( ${SAMP} ${Boost_LIBRARIES} )
target_link_libraries( ${SAMP} ${OPENSSL_LIBRARIES} )
target_link_libraries( ${SAMP} ${ZLIB_LIBRARIES} )
target_link_libraries (${SAMP} ${CMAKE_THREAD_LIBS_INIT})
