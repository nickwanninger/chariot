#!/bin/bash


pushd () {
    command pushd "$@" > /dev/null
}

popd () {
    command popd "$@" > /dev/null
}


DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
source $DIR/../.config

ROOT=$(realpath $DIR/..)
SYSROOT=$ROOT/build/root
TOOLCHAIN=$ROOT/toolchain/local/bin
export PATH=$TOOLCHAIN:$PATH

pushd ../
	tools/sysroot.sh
popd


FREETYPE_NAME=freetype-2.10.1

mkdir -p src
pushd src

	# Download freetype if needed
	if [ ! -d $FREETYPE_NAME ]; then
		wget "https://download.savannah.gnu.org/releases/freetype/$FREETYPE_NAME.tar.gz"
		tar -xzvf $FREETYPE_NAME.tar.gz
		rm $FREETYPE_NAME.tar.gz
		pushd $FREETYPE_NAME
			patch -p1 < ../../patches/freetype.patch
		popd
	fi



	# if [ ! -d chariot-sdl ]; then
	# 	git clone git@github.com:nickwanninger/chariot-sdl.git --depth 1
	# fi
popd


for ARCH in "$@"
do


	mkdir -p build/$ARCH
	pushd build/$ARCH



		if [ ! -d cairo ]; then
			git clone git@github.com:freedesktop/cairo.git --depth 1 cairo
			pushd cairo
			popd
		fi

		pushd cairo
			./autogen.sh

			./configure                                                 \
					--host=$ARCH-elf                                        \
					--prefix=$ROOT/ports/out/$ARCH/                         \
					--disable-shared                                        \
					--enable-ps=no --enable-pdf=no --enable-win32-font=no   \
					--enable-win32=no --enable-quartz-font=no               \
					--enable-quartz=no --enable-xlib-xrender=no --enable-xlib=no \
					--enable-ft-font=no                                                \
					"CFLAGS=-I$ROOT/include -fno-stack-protector -nostdlib -s " \
					"CXXFLAGS=-I$ROOT/include -fno-stack-protector -DUSERLAND -nostdlib -s "
			

			make -j
			mkdir -p $ROOT/ports/out/$ARCH/lib/
			cp src/.libs/libcairo.a $ROOT/ports/out/$ARCH/lib/
		popd

		mkdir -p freetype
		pushd freetype
			$ARCH-elf-gcc -v

			$ROOT/ports/src/$FREETYPE_NAME/configure                    \
					--host=$ARCH-elf                                        \
					--prefix=$ROOT/ports/out/$ARCH/                         \
					--with-harfbuzz=no                                      \
					--with-bzip2=no                                         \
					--with-zlib=no                                          \
					--with-png=no                                           \
					--disable-shared                                        \
					"CFLAGS=-I$ROOT/include -fno-stack-protector -DUSERLAND -nostdlib -s " \
					"CXXFLAGS=-I$ROOT/include -fno-stack-protector -DUSERLAND -nostdlib -s "

			
			make -j
			make install

		popd
	popd

done
