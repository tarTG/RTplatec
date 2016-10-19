cmake_minimum_required(VERSION 2.0)

project(RTplacet)
option(INTEGRATION_TEST "Build integration test" OFF)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(LibNoise REQUIRED)


include_directories("include" )
include_directories("3rdParty/noiseutils" )
include_directories("3rdParty/GL3W" )

include_directories("${LIBNOISE_INCLUDE_DIR}" )

add_definitions( -std=c++14 )

add_subdirectory("3rdParty")
add_subdirectory("src")

    add_subdirectory("test")
