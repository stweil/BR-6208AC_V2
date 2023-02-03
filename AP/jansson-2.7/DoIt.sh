#!/bin/sh
. ../../define/PATH

if [ $# = 1 ]; then
    case "$1" in
        "clean")
            MAKECLEAR=1
            MAKING=0
            ;;
        "make")
            MAKECLEAR=0
            MAKING=1
			;;
    esac
fi	

if [ ! -f Makefile ]; then 
	CC=${CROSS_COMPILE}gcc  ./configure --host=mipsel-linux --build=i686-linux --prefix=$JANSSONDIR/_install --disable-static
fi
#exit

if [ "$MAKECLEAR" = 1 ]; then
	make clean
    if [ $? != 0 ]; then
	    exit 1
	fi					
fi
if [ "$MAKING" = 1 ]; then
	make
	make install
    if [ $? != 0 ]; then
	    exit 1
	fi					
fi
