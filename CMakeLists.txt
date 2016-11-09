#https://cmake.org/Wiki/CMake_Useful_Variables
cmake_minimum_required (VERSION 2.8)
SET(CMAKE_CONFIGURATION_TYPES "Debug;Release;MinSizeRel;RelWithDebInfo" CACHE STRING "" FORCE)
project (PHD)

set (CMAKE_CXX_FLAGS_DEBUG "-Wall -g -O0 -DPIC")
set (CMAKE_C_FLAGS_DEBUG "-Wall -g -O0 -DPIC")

# check compilers for c++11
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fpic")
elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -fpic")
else()
	message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()


#version number
set (PHD_VERSION_MAJOR 1)
set (PHD_VERSION_MINOR 0)

add_subdirectory (${CMAKE_SOURCE_DIR}/libatari )
add_subdirectory (${CMAKE_SOURCE_DIR}/unittest )






