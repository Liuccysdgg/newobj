# CMAKE最低版本号要求
cmake_minimum_required(VERSION 2.8)


########################  项目名称  #######################  
set(PROJECT_NAME newobj)
project(${PROJECT_NAME})



########################  开发选择  #######################  
#### 编译DEBUG ####
set(BUILD_DEBUG TRUE)
#### 语言标准 ####
set(BUILD_LANG "-std=c++17")
#### 64位 ####
set(BUILD_X64 TRUE)
########################  功能选择  #######################  
#========  使用功能[结算]  =========

if(BUILD_DEBUG)
    set(DEBUG_FLAGS "-g")
	set(NAME_DR "_debug")
else()
    #set(DEBUG_FLAGS "-O2")
	set(NAME_DR "_release")
endif()
if(BUILD_X64)
	set(NAME_X64 "_x64")
else()
	set(NAME_X64 "_x86")
	set(FLAGS_X64 "-m32")
endif()
set(FLAGS_GEN "-DNEWOBJ_STATIC -DLINUX")

########################  编译器  ########################  
SET(CMAKE_SYSTEM_NAME Linux)
#SET(CMAKE_C_COMPILER "arm-linux-gnueabihf-gcc")
#SET(CMAKE_CXX_COMPILER "arm-linux-gnueabihf-g++")
SET(CMAKE_C_COMPILER "gcc")
SET(CMAKE_CXX_COMPILER "g++")



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

########################  编译选项  ########################  
IF (WIN32)
	MESSAGE(STATUS "================WIN32================")
	set(CMAKE_CXX_FLAGS "/wd\"4819\" /wd\"4996\"")
ELSEIF (APPLE)
	MESSAGE(STATUS "================APPLE================")
ELSEIF (UNIX)
	MESSAGE(STATUS "================UNIX================")
	set(CMAKE_CXX_FLAGS "${DEBUG_FLAGS} ${FLAGS_X64} ${FLAGS_GEN} ${BUILD_LANG}" )
ENDIF ()

########################  生成路径  ########################  
set(LIBRARY_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/bin/)    


#######################  生成目标  ########################  
add_library(
	"${PROJECT_NAME}${NAME_X64}${NAME_DR}"
	STATIC ${DB_SRC} ${LOG_SRC} ${NETWORK_SRC} ${PUBLIC_SRC} ${UTIL_SRC} ${TEMPLATE_SRC} ${OTHER_SRC} ${MGR_SRC} ${SDK_SRC} ${SDK_CODEC_SRC})
#######################  其它项目  ########################  
add_subdirectory(module/network)
add_subdirectory(thirdparty/mysql)
add_subdirectory(thirdparty/redis)
add_subdirectory(thirdparty/clickhouse)

