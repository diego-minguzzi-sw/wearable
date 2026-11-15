project(poc_gps_over_serial LANGUAGES CXX)

file(GLOB PROJ_SOURCES  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"	)
file(GLOB PROJ_HEADERS  "${CMAKE_CURRENT_SOURCE_DIR}/*.h"	)

add_executable(poc_gps_over_serial ${PROJ_SOURCES} ${PROJ_HEADERS})
