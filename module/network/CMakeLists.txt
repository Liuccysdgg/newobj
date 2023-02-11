# CMAKE��Ͱ汾��Ҫ��?
cmake_minimum_required(VERSION 2.8)
########################  ��Ŀ����  #######################  
set(LIBRARY_NAME module_network)


########################  ����ѡ��  #######################  
#### ����DEBUG ####
set(BUILD_DEBUG TRUE)
#### ���Ա�׼ ####
set(BUILD_LANG "-std=c++17")
#### 64λ ####
set(BUILD_X64 TRUE)
########################  ����ѡ��  #######################  
#========  ʹ�ù���[����]  =========

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
########################  ������  ########################  
SET(CMAKE_SYSTEM_NAME Linux)
#SET(CMAKE_C_COMPILER "arm-linux-gnueabihf-gcc")
#SET(CMAKE_CXX_COMPILER "arm-linux-gnueabihf-g++")
SET(CMAKE_C_COMPILER "gcc")
SET(CMAKE_CXX_COMPILER "g++")

########################  ͷ�ļ�  ########################  
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/thirdparty)
include_directories(${PROJECT_SOURCE_DIR}/module)
include_directories(${PROJECT_SOURCE_DIR}/include)

if(LIB_QT)
	if(QT5)
		include_directories(${PROJECT_SOURCE_DIR}/include/qt/5.9.9)
	else()
		include_directories(${PROJECT_SOURCE_DIR}/include/qt/4.8.5)
	endif()
endif()

########################  Դ�ļ�  ########################  
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

########################  ����ѡ��  ########################  
IF (WIN32)
	MESSAGE(STATUS "================WIN32================")
	set(CMAKE_CXX_FLAGS "/wd\"4819\" /wd\"4996\"")
ELSEIF (APPLE)
	MESSAGE(STATUS "================APPLE================")
ELSEIF (UNIX)
	MESSAGE(STATUS "================UNIX================")
	set(CMAKE_CXX_FLAGS "${DEBUG_FLAGS} ${FLAGS_X64} ${FLAGS_GEN} ${BUILD_lANG}" )
ENDIF ()



#######################  ����Ŀ��  ########################  
add_library(
	"${LIBRARY_NAME}${NAME_X64}${NAME_DR}"
	STATIC ${MODULE_NETWORK_SRC} ${MODULE_NETWORK_UDP_SRC} ${MODULE_NETWORK_FTP_SRC} ${MODULE_NETWORK_HTTP_SRC} ${MODULE_NETWORK_TCP_SRC} ${MODULE_NETWORK_OTHER_SRC})
	
