cmake_minimum_required(VERSION 2.8)

set(LIBRARY_NAME thirdparty_mysql)


include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/thirdparty)
include_directories(${PROJECT_SOURCE_DIR}/module)
include_directories(${PROJECT_SOURCE_DIR}/include)
aux_source_directory(SRC_CORE ${PROJECT_SOURCE_DIR}/thirdparty/mysql/*.cpp)

file(GLOB SRC_CORE "${PROJECT_SOURCE_DIR}/thirdparty/mysql/*.*")



add_library(
	"${LIBRARY_NAME}_${CMAKE_BUILD_TYPE}"
	STATIC "${SRC_CORE}")
	
