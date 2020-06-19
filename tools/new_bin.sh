#!/bin/bash


if [ -z "$1" ];
then
		echo "usage: make_prog.sh <name>"
		exit 1
fi



mkdir -p user/bin/$1

touch user/bin/$1/CMakeLists.txt
echo 'file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.c *.asm)' >> user/bin/$1/CMakeLists.txt
echo "chariot_bin($(basename $1))" >> user/bin/$1/CMakeLists.txt

touch user/bin/$1/main.cpp