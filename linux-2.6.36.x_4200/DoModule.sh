#!/bin/sh
. ../define/PATH
. ../define/FUNCTION_SCRIPT
PATH=${CROSS_BIN}:$PATH

if [ "$1" != "make" ] && [ "$1" != "clean" ] && [ "$1" != "all" ]; then
	echo "$0 [ make | clean | all ]"
	exit 1
fi

if [ "$1" = "clean" ] || [ "$1" = "all" ]; then
	make V=1 ARCH=mips CROSS_COMPILE=${CROSS_COMPILE} -j1 V=1 modules clean
	if [ $? = 1 ]; then exit 1; fi
fi

if [ "$1" = "make" ] || [ "$1" = "all" ]; then
	make ARCH=mips CROSS_COMPILE=${CROSS_COMPILE} -j1 V=1 modules
	if [ $? != 0 ]; then exit 1; fi
fi
