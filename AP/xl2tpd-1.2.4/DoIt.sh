#!/bin/sh
if [ "$1" != "configure" ] && [ "$1" != "clean" ] && [ "$1" != "make" ] && [ "$1" != "install" ] && [ "$1" != "all" ]; then
	echo "$0 [ configure | clean | make | install | all ]"
	exit 1
fi

. ../../define/PATH
. ${TOOLS_DIR}/script_tools.sh
PATH=${CROSS_BIN}:$PATH

if [ "$1" = "configure" ] || [ "$1" = "all" ]; then
	:
fi

if [ "$1" = "clean" ] || [ "$1" = "all" ]; then
	make clean
	if [ $? = 1 ]; then exit 1; fi
fi

if [ "$1" = "make" ] || [ "$1" = "all" ]; then
	make KERNELSRC=${LINUX_DIR} CC=${CROSS_COMPILE}gcc
	if [ $? = 1 ]; then exit 1; fi
	${CROSS_COMPILE}strip xl2tpd
	if [ $? = 1 ]; then exit 1; fi
fi

if [ "$1" = "install" ] || [ "$1" = "all" ]; then
	cp -f xl2tpd ${ROMFS_DIR}/bin/l2tpd
	if [ $? = 1 ]; then exit 1; fi
	
fi

# #define CONTROL_PIPE "/var/run/xl2tpd/l2tp-control"
# /var/run/l2tp-control
