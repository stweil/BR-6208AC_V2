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
	make
	if [ $? = 1 ]; then exit 1; fi
fi

if [ "$1" = "install" ] || [ "$1" = "all" ]; then
	mkdir -p ${ROMFS_DIR}/bin
	cp -f lld2d ${ROMFS_DIR}/bin
	if [ $? = 1 ]; then exit 1; fi
	
	if [ "$_LLTD_ICO_CUSTOMIZE_" = "y" ]; then
		mkdir -p ${ROMFS_DIR}/etc
		if [ "$_RFTYPE_" = "2T2R" ]; then
			cp src/BR-6424n.ico ${ROMFS_DIR}/etc/icon.ico
			if [ $? = 1 ]; then exit 1; fi
		elif [ "$_RFTYPE_" = "1T1R" ]; then
			cp src/BR-6225n.ico ${ROMFS_DIR}/etc
			if [ $? = 1 ]; then exit 1; fi
			cp src/BR-6226n.ico ${ROMFS_DIR}/etc
			if [ $? = 1 ]; then exit 1; fi
		fi
	fi
fi