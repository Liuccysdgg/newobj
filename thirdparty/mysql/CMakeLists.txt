cmake_minimum_required(VERSION 2.8)

set(LIBRARY_NAME thirdparty_mysql)


set(BUILD_DEBUG TRUE)
set(BUILD_LANG "-std=c++17")
set(BUILD_X64 TRUE)

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
set(FLAGS_GEN "-DNEWOBJ_STATIC")
SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_C_COMPILER "gcc")
SET(CMAKE_CXX_COMPILER "g++")

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/thirdparty)
include_directories(${PROJECT_SOURCE_DIR}/module)
include_directories(${PROJECT_SOURCE_DIR}/include)
aux_source_directory(SRC_CORE ${PROJECT_SOURCE_DIR}/thirdparty/mysql/*.cpp)

file(GLOB SRC_CORE "${PROJECT_SOURCE_DIR}/thirdparty/mysql/*.*")

IF (WIN32)
	MESSAGE(STATUS "================WIN32================")
	set(CMAKE_CXX_FLAGS "/wd\"4819\" /wd\"4996\"")
ELSEIF (APPLE)
	MESSAGE(STATUS "================APPLE================")
ELSEIF (UNIX)
	MESSAGE(STATUS "================UNIX================")
	set(CMAKE_CXX_FLAGS "${DEBUG_FLAGS} ${FLAGS_X64} ${FLAGS_GEN}" )
ENDIF ()


add_library(
	"${LIBRARY_NAME}${NAME_X64}${NAME_DR}"
	STATIC "${SRC_CORE}")
	
