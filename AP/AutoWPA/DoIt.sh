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

# make 之前也要先 clean
if [ "$1" = "make" ] || [ "$1" = "clean" ] || [ "$1" = "all" ]; then
	make clean
	if [ $? = 1 ]; then exit 1; fi
fi

if [ "$1" = "make" ] || [ "$1" = "all" ]; then
	make _GENERATE_ENCRYPTSEED_USEMD5_="${_GENERATE_ENCRYPTSEED_USEMD5_}"
	if [ $? = 1 ]; then exit 1; fi
fi

if [ "$1" = "install" ] || [ "$1" = "all" ]; then
	mkdir -p ${ROMFS_DIR}/bin
	cp -f AutoWPA ${ROMFS_DIR}/bin
	if [ $? = 1 ]; then exit 1; fi
fi