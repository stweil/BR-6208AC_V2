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

if [ "$MAKECLEAR" = "1" ]; then
	make clean

	if [ $? != 0 ]; then
		exit 1
	fi				
fi

if [ "$MAKING" = "1" ]; then
	make clean

	CC=${CROSS_COMPILE}gcc \
	AR=${CROSS_COMPILE}ar \
	LD=${CROSS_COMPILE}ld \
	NM=${CROSS_COMPILE}nm \
	OBJCOPY=${CROSS_COMPILE}objcopy \
	OBJDUMP=${CROSS_COMPILE}objdump \
	STRIP=${CROSS_COMPILE}strip \
	RANLIB=${CROSS_COMPILE}ranlib \
	CXX=${CROSS_COMPILE}g++ \
	make
	#CC=$CROSS"gcc" STRIP=$CROSS"strip" make
	#make install

	if [ $? != 0 ]; then
		exit 1
	fi
fi
