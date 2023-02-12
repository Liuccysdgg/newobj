# CMAKE最低版本号要求
cmake_minimum_required(VERSION 2.8)


########################  项目名称  #######################  
set(PROJECT_NAME newobj)
project(${PROJECT_NAME})



if(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))                                                                                                                                                    
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")
elseif(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Release"))

endif()



add_definitions(-DNEWOBJ_STATIC -std=c++17 -Wall)


########################  头文件  ########################  
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/thirdparty)
include_directories(${PROJECT_SOURCE_DIR}/module)
include_directories(${PROJECT_SOURCE_DIR}/include)
########################  源文件  ########################  
aux_source_directory(ALGORITHM_SRC ${PROJECT_SOURCE_DIR}/src/algorithm/*.cpp)
aux_source_directory(DB_SRC ${PROJECT_SOURCE_DIR}/src/db/*.cpp)
aux_source_directory(LOG_SRC ${PROJECT_SOURCE_DIR}/src/log/*.cpp)
aux_source_directory(PUBLIC_SRC ${PROJECT_SOURCE_DIR}/src/public/*.cpp)
aux_source_directory(UTIL_SRC ${PROJECT_SOURCE_DIR}/src/util/*.cpp)
aux_source_directory(TEMPLATE_SRC ${PROJECT_SOURCE_DIR}/src/template/*.cpp)
aux_source_directory(OTHER_SRC ${PROJECT_SOURCE_DIR}/src/other/*.cpp)
aux_source_directory(SDK_SRC ${PROJECT_SOURCE_DIR}/src/sdk/*.cpp)
aux_source_directory(SDK_CODEC_SRC ${PROJECT_SOURCE_DIR}/src/sdk/codec/*.cpp)
aux_source_directory(MGR_SRC ${PROJECT_SOURCE_DIR}/src/mgr/*.cpp)

file(GLOB ALGORITHM_SRC " ${PROJECT_SOURCE_DIR}/src/algorithm/*.*")
file(GLOB DB_SRC "src/db/*.*")
file(GLOB LOG_SRC "src/log/*.*")
file(GLOB PUBLIC_SRC  "src/public/*.*")
file(GLOB UTIL_SRC "src/util/*.*")
file(GLOB TEMPLATE_SRC "src/template/*.*")
file(GLOB OTHER_SRC "src/other/*.*")
file(GLOB SDK_SRC "src/sdk/*.*")
file(GLOB SDK_CODEC_SRC "src/sdk/codec/*.*")
file(GLOB MGR_SRC "src/mgr/*.*")

########################  生成路径  ########################  
set(LIBRARY_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/bin/)    
#######################  生成目标  ########################  
add_library(
    "${PROJECT_NAME}_${CMAKE_BUILD_TYPE}"
	STATIC ${DB_SRC} ${LOG_SRC} ${NETWORK_SRC} ${PUBLIC_SRC} ${UTIL_SRC} ${TEMPLATE_SRC} ${OTHER_SRC} ${MGR_SRC} ${SDK_SRC} ${SDK_CODEC_SRC})
#######################  其它项目  ########################  
add_subdirectory(module/network)
add_subdirectory(thirdparty/mysql)
add_subdirectory(thirdparty/redis)
add_subdirectory(thirdparty/clickhouse)

