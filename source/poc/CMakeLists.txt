cmake_minimum_required(VERSION ${MIN_CMAKE_VERSION})

if(BUILD_POC)
	add_subdirectory(poc_gps_over_serial)
endif()
