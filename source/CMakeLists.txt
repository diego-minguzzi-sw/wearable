set( MIN_CMAKE_VERSION 3.20)
cmake_minimum_required(VERSION ${MIN_CMAKE_VERSION})

project(wearable LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_POC "Build the proof of concepts POC(s)" ON)

if(BUILD_POC)
	add_subdirectory(poc)
endif()
