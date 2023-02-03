#!/bin/sh
if [ "$1" != "configure" ] && [ "$1" != "clean" ] && [ "$1" != "make" ] && [ "$1" != "install" ] && [ "$1" != "all" ]; then
	echo "$0 [ configure | clean | make | install | all ]"
	exit 1
fi

. ../../define/PATH
. ../../define/FUNCTION_SCRIPT
PATH=${CROSS_BIN}:$PATH

if [ "$1" = "configure" ] || [ "$1" = "all" ]; then
	:
fi

if [ "$1" = "clean" ] || [ "$1" = "all" ]; then
	make clean
	if [ $? = 1 ]; then exit 1; fi
fi

if [ "$1" = "make" ] || [ "$1" = "all" ]; then
	make CC=${CROSS_COMPILE}gcc AR=${CROSS_COMPILE}ar RANLIB=${CROSS_COMPILE}ranlib BUILD_STRIPPING=y BUILD_NOLIBM=y
	if [ $? = 1 ]; then exit 1; fi
fi

if [ "$1" = "install" ] || [ "$1" = "all" ]; then
	mkdir -p ${ROMFS_DIR}/lib
	mkdir -p ${ROMFS_DIR}/bin
	cp -f libiw.so.29 ${ROMFS_DIR}/lib
	if [ $? = 1 ]; then exit 1; fi
	
	cp -f iwpriv ${ROMFS_DIR}/bin
	if [ $? = 1 ]; then exit 1; fi
	
fi