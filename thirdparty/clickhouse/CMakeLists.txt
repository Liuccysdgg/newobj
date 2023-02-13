cmake_minimum_required(VERSION 2.8)

set(LIBRARY_NAME thirdparty_clickhouse)



file(GLOB SRC_CORE "${PROJECT_SOURCE_DIR}/thirdparty/clickhouse/*.*")

include_directories(${PROJECT_SOURCE_DIR}/thirdparty/clickhouse/include)

add_library(
	"${LIBRARY_NAME}_${BUILD_TYPE}"
	STATIC "${SRC_CORE}")
	
