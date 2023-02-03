#!/bin/sh
. ../../define/PATH

if [ "$1" = "make" ] || [ "$1" = "clean" ] || [ "$1" = "all" ]; then
        make clean
        if [ $? = 1 ]; then exit 1; fi
fi

if [ "$1" = "make" ] || [ "$1" = "all" ]; then
        make CC=${CROSS_COMPILE}gcc 
	${CROSS_COMPILE}strip noip2
        if [ $? = 1 ]; then exit 1; fi
fi

if [ "$1" = "install" ] || [ "$1" = "all" ]; then
	cp -f noip2 ${ROMFS_DIR}/bin
        if [ $? = 1 ]; then exit 1; fi
fi

