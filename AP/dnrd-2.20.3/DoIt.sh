#!/bin/sh
if [ "$1" != "configure" ] && [ "$1" != "clean" ] && [ "$1" != "make" ] && [ "$1" != "install" ] && [ "$1" != "all" ]; then
	echo "$0 [ configure | clean | make | install | all ]"
	exit 1
fi

. ../../define/PATH
. ../../define/FUNCTION_SCRIPT
PATH=${CROSS_BIN}:$PATH

if [ "$1" = "configure" ] || [ "$1" = "all" ] || [ ! -f "Makefile" ]; then
#		./configure --host=${CROSS_HOST} --prefix=/ --sysconfdir=/etc CFLAGS+=-D$HOSTNAME
	if [ "$_IQv2_" ]; then
		if [ "$_IQv2_" -a "$_WIFI_REPEATER_" = "y" ]; then
			./configure --host=${CROSS_HOST} --prefix=/ --sysconfdir=/etc CFLAGS="-g -O2 -D_IQv2_ -D_WIFI_REPEATER_"
		else
			./configure --host=${CROSS_HOST} --prefix=/ --sysconfdir=/etc CFLAGS="-g -O2 -D_IQv2_"
		fi
	else
		./configure --host=${CROSS_HOST} --prefix=/ --sysconfdir=/etc
	fi
fi

if [ "$1" = "clean" ] || [ "$1" = "all" ]; then
	make distclean
	if [ $? = 1 ]; then exit 1; fi
fi

if [ "$1" = "make" ] || [ "$1" = "all" ]; then
	make _HOSTNAME_=$HOSTNAME
	if [ $? = 1 ]; then exit 1; fi

	${CROSS_COMPILE}strip src/dnrd
	if [ $? = 1 ]; then exit 1; fi
fi

if [ "$1" = "install" ] || [ "$1" = "all" ]; then
	mkdir -p ${ROMFS_DIR}/bin
	mkdir -p ${ROMFS_DIR}/etc/dnrd
	cp src/dnrd ${ROMFS_DIR}/bin
	if [ $? = 1 ]; then exit 1; fi
fi
