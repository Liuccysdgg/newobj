cmake_minimum_required(VERSION 2.8)
set(LIBRARY_NAME module_network)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/thirdparty)
include_directories(${PROJECT_SOURCE_DIR}/module)
include_directories(${PROJECT_SOURCE_DIR}/include)

aux_source_directory(MODULE_NETWORK_SRC ${PROJECT_SOURCE_DIR}/module/network/*.cpp)
aux_source_directory(MODULE_NETWORK_UDP_SRC ${PROJECT_SOURCE_DIR}/module/network/udp/*.cpp)
aux_source_directory(MODULE_NETWORK_FTP_SRC ${PROJECT_SOURCE_DIR}/module/network/ftp/*.cpp)
aux_source_directory(MODULE_NETWORK_HTTP_SRC ${PROJECT_SOURCE_DIR}/module/network/http/*.cpp)
aux_source_directory(MODULE_NETWORK_TCP_SRC ${PROJECT_SOURCE_DIR}/module/network/tcp/*.cpp)
aux_source_directory(MODULE_NETWORK_OTHER_SRC ${PROJECT_SOURCE_DIR}/module/network/other/*.cpp)

file(GLOB MODULE_NETWORK_SRC "${PROJECT_SOURCE_DIR}/module/network/*.*")
file(GLOB MODULE_NETWORK_UDP_SRC "${PROJECT_SOURCE_DIR}/module/network/udp/*.*")
file(GLOB MODULE_NETWORK_FTP_SRC "${PROJECT_SOURCE_DIR}/module/network/ftp/*.*")
file(GLOB MODULE_NETWORK_HTTP_SRC "${PROJECT_SOURCE_DIR}/module/network/http/*.*")
file(GLOB MODULE_NETWORK_TCP_SRC  "${PROJECT_SOURCE_DIR}/module/network/tcp/*.*")
file(GLOB MODULE_NETWORK_OTHER_SRC "${PROJECT_SOURCE_DIR}/module/network/other/*.*")


add_library(
	"${LIBRARY_NAME}_${BUILD_TYPE}"
	STATIC ${MODULE_NETWORK_SRC} ${MODULE_NETWORK_UDP_SRC} ${MODULE_NETWORK_FTP_SRC} ${MODULE_NETWORK_HTTP_SRC} ${MODULE_NETWORK_TCP_SRC} ${MODULE_NETWORK_OTHER_SRC})
	
