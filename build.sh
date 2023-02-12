#!/bin/bash
print(){
    if [ "$1"x == "blue"x ]; then
        echo -e "\e[34m $2 \e[0m"
    elif [ "$1"x == "red"x ]; then
        echo -e "\e[31m $2 \e[0m"
    elif [ "$1"x == "yellow"x ]; then
        echo -e "\e[33m $2 \e[0m"
    elif [ "$1"x == "black"x ]; then
        echo -e "\e[30m $2 \e[0m"
    elif [ "$1"x == "green"x ]; then
        echo -e "\e[32m $2 \e[0m"
    elif [ "$1"x == "purple"x ]; then
        echo -e "\e[35m $2 \e[0m"
    elif [ "$1"x == "cyan"x ]; then
        echo -e "\e[36m $2 \e[0m"
    elif [ "$1"x == "white"x ]; then
        echo -e "\e[37m $2 \e[0m"
    fi
}
build(){
    print blue "***************** Build [$1] Waiting... *****************"
    cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=$1
    make -j8
    cp ../bin/* ../../bin/
}
clear_build(){
    print yellow "***************** Clear  *****************"
    rm -rf CMakeFiles
    rm -rf cmake_install.cmake
    rm -rf Makefile
    rm -rf CMakeCache.txt
    echo "clear over"

}
help_pr(){
    print purple "*******************帮助文档********************"
    print purple "./build clear     :       清理编译内容"
    print purple "./build release   :       编译release版本"
    print purple "./build debug     :       编译debug版本"
    print purple "./build re        :       清理并重新编译"
    print purple "./build           :       编译release和debug版本"
}
mkdir build
cd build
if [ "$#" -eq "0"  ];then
    build "Debug"
    build "Release"
elif [ "$#" -eq "1" ]; then
    if [ "$1"x == "clear"x ]; then
        clear_build
    elif [ "$1"x == "debug"x ]; then
        build "Debug"
    elif [ "$1"x == "release"x ]; then
        build "Release"
    elif [ "$1"x == "re"x ]; then
        clear_build
        build "Debug"
        build "Release"
    else
        help_pr
    fi
else
    help_pr
fi



