#!/bin/sh
if [ "$1" != "configure" ] && [ "$1" != "clean" ] && [ "$1" != "make" ] && [ "$1" != "install" ] && [ "$1" != "all" ]; then
	echo "$0 [ configure | clean | make | install | all ]"
	exit 1
fi

. ../../define/PATH
. ../../define/FUNCTION_SCRIPT
PATH=${CROSS_BIN}:$PATH

if [ "$1" = "configure" ] || [ "$1" = "all" ]; then
CC=${CROSS_COMPILE}gcc ./configure --host=mipsel-linux --with-kernel=${LINUX_DIR} --enable-static
	:
	if [ $? = 1 ]; then exit 1; fi
fi

if [ "$1" = "clean" ] || [ "$1" = "all" ]; then
	make clean
	if [ $? = 1 ]; then exit 1; fi
fi

if [ "$1" = "make" ] || [ "$1" = "all" ]; then
	if [ $? = 1 ]; then exit 1; fi
	make CC=${CROSS_COMPILE}gcc ROOTDIR="${ROOT_DIR}" LINUXDIR="`echo $LINUX_DIR | awk -F "/" '{print $(NF)}'`" CONFIG_CROSS_COMPILER_PATH="${CROSS_BIN}"
	${CROSS_COMPILE}strip .libs/iptables-multi
	if [ $? = 1 ]; then exit 1; fi
fi

if [ "$1" = "install" ] || [ "$1" = "all" ]; then
	mkdir -p ${ROMFS_DIR}/bin
	cp -f .libs/iptables-multi ${ROMFS_DIR}/bin/iptables
	cp -f ./libiptc/.libs/libip4tc.so.0 ${ROMFS_DIR}/lib/libip4tc.so.0
	cp -f ./.libs/libxtables.so.5 ${ROMFS_DIR}/lib/libxtables.so.5	
	if [ $? = 1 ]; then exit 1; fi
fi


if [ "$1" != "configure" ] && [ "$1" != "clean" ] && [ "$1" != "make" ] && [ "$1" != "install" ] && [ "$1" != "all" ]; then
	echo "$0 [ configure | clean | make | install | all ]"
	exit 1
fi
