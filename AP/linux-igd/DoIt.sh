#!/bin/sh
if [ "$1" != "configure" ] && [ "$1" != "clean" ] && [ "$1" != "make" ] && [ "$1" != "install" ] && [ "$1" != "all" ]; then
	echo "$0 [ configure | clean | make | all ]"
	exit 1
fi

. ../../define/PATH
PATH=${CROSS_BIN}:$PATH

if [ "$1" = "configure" ] || [ "$1" = "all" ]; then
	:
fi

if [ "$1" = "clean" ] || [ "$1" = "all" ]; then
	make clean
	if [ $? = 1 ]; then exit 1; fi
fi

if [ "$1" = "make" ] || [ "$1" = "all" ]; then
	make\
	CC=${CROSS_COMPILE}gcc\
	STRIP=${CROSS_COMPILE}strip\
	INCLUDES="-I${APPLICATIONS_DIR}/lib/libupnp/ixml/inc -I${APPLICATIONS_DIR}/lib/libupnp/upnp/inc -I${APPLICATIONS_DIR}/lib/libupnp/threadutil/inc"\
	CFLAGS="-Wall -g -O2 -I${LINUX_DIR}/drivers/net/raeth"\
	LIB_PATH=${APPLICATIONS_DIR}/lib/lib
	if [ $? = 1 ]; then exit 1; fi
fi

if [ "$1" = "install" ] || [ "$1" = "all" ]; then
	mkdir -p ${ROMFS_DIR}/usr/sbin
	cp -f upnpd ${ROMFS_DIR}/usr/sbin
	if [ $? = 1 ]; then exit 1; fi
#	cp -f upnp_xml.sh ${ROMFS_DIR}/bin
#	if [ $? = 1 ]; then exit 1; fi
	mkdir -p ${ROMFS_DIR}/etc/linuxigd
	cp -f etc/ligd.gif ${ROMFS_DIR}/etc/linuxigd
	if [ $? = 1 ]; then exit 1; fi
	cp -f etc/upnpd.conf ${ROMFS_DIR}/etc/linuxigd
	if [ $? = 1 ]; then exit 1; fi
fi
# From Ralink SDK 3.3.0.0
#
# globals.h 改路徑
#   #define CONF_FILE "/etc/linuxigd/upnpd.conf"
#
# etc/upnpd.conf 改bitrate
#   upstream_bitrate = 100000000
#   downstream_bitrate = 100000000
#
# upnp_xml.sh，於每次開機時自動組合 XML，依照 Model 變更
