#!/bin/sh
if [ "$1" != "configure" ] && [ "$1" != "clean" ] && [ "$1" != "make" ] && [ "$1" != "install" ] && [ "$1" != "all" ]; then
	echo "$0 [ configure | clean | make | install | all ]"
	exit 1
fi

. ../../define/PATH
PATH=${CROSS_BIN}:$PATH

chmod 777 *

if [ "$1" = "make" ] || [ "$1" = "clean" ] || [ "$1" = "configure" ] || [ "$1" = "all" ]; then
	./configure --host=${CROSS_HOST} CC=${CROSS_COMPILE}gcc
fi

if [ "$1" = "make" ] || [ "$1" = "clean" ] || [ "$1" = "all" ]; then
	make clean
	if [ $? = 1 ]; then exit 1; fi
fi

if [ "$1" = "make" ] || [ "$1" = "all" ]; then
	make
	if [ $? = 1 ]; then exit 1; fi
	${CROSS_COMPILE}strip nbtscan
	if [ $? = 1 ]; then exit 1; fi
fi

if [ "$1" = "install" ] || [ "$1" = "all" ]; then
	mkdir -p ${ROMFS_DIR}/bin
	cp -f nbtscan ${ROMFS_DIR}/bin
	if [ $? = 1 ]; then exit 1; fi
fi