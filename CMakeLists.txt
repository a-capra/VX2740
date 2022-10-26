# CMake for CAEN VX2740 reader
#
#
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(VX2740reader)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}" CACHE PATH "..." FORCE)
endif()

set(CMAKE_CXX_STANDARD 11)

##########################################
# Find ZLIB
find_package(ZLIB REQUIRED)

set(Sources ${PROJECT_SOURCE_DIR}/midasio/midasio.cxx ${PROJECT_SOURCE_DIR}/midasio/lz4frame.cxx
    ${PROJECT_SOURCE_DIR}/midasio/lz4.cxx ${PROJECT_SOURCE_DIR}/midasio/lz4hc.cxx ${PROJECT_SOURCE_DIR}/midasio/xxhash.cxx
    ${PROJECT_SOURCE_DIR}/TVX2740RawData.cxx)

add_executable(vx2740reader.exe reader.cxx ${Sources})

target_compile_options(vx2740reader.exe PUBLIC -Wall -Wextra -pedantic)
target_include_directories(vx2740reader.exe PUBLIC ${PROJECT_SOURCE_DIR}/midasio)
target_link_libraries(vx2740reader.exe PUBLIC ${ZLIB_LIBRARIES})

install(
  TARGETS vx2740reader.exe
  DESTINATION ${CMAKE_INSTALL_PREFIX}
  )

message(STATUS "${PROJECT_NAME} ${CMAKE_BUILD_TYPE} build")
message(STATUS "vx2740reader.exe will be installed in ${CMAKE_INSTALL_PREFIX}")
