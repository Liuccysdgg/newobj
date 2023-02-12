cmake_minimum_required(VERSION 2.8)

set(LIBRARY_NAME thirdparty_redis)


include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/thirdparty)
include_directories(${PROJECT_SOURCE_DIR}/module)
include_directories(${PROJECT_SOURCE_DIR}/include)
aux_source_directory(SRC_CORE ${PROJECT_SOURCE_DIR}/thirdparty/redis/*.cpp)
file(GLOB SRC_CORE "${PROJECT_SOURCE_DIR}/thirdparty/redis/*.*")


add_library(
	"${LIBRARY_NAME}_${CMAKE_BUILD_TYPE}"
	STATIC "${SRC_CORE}")
	
