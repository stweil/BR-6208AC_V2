#!/bin/sh
if [ "$1" != "configure" ] && [ "$1" != "clean" ] && [ "$1" != "make" ] && [ "$1" != "install" ] && [ "$1" != "all" ]; then
	echo "$0 [ configure | clean | make | install | all ]"
	exit 1
fi

. ../../define/PATH
. ../../define/FUNCTION_SCRIPT
. ${TOOLS_DIR}/script_tools.sh
PATH=${CROSS_BIN}:$PATH

if [ "$1" = "make" ] || [ "$1" = "configure" ] || [ "$1" = "all" ]; then
	if [ "$_PCI_UPNP_CONFIGURE_" = "y" ]; then
		CFLAGS="-Os -Wall -D_PCI_UPNP_CONFIGURE_"
	else
		CFLAGS="-Os -Wall"
	fi

	if [ ! -e .config ];then
		cp .config.mips .config
	fi

	if [ "$_SITECOM_AUTO_FW_UPGRADE_" = "y" ]; then
		cp .config.sitecomautofw .config
	fi

	if [ ! -e libupnp/makefile ];then
		cp .makefile.mips libupnp/makefile	
	fi
	

	SEARCH_AND_REPLACE libupnp/makefile "^	AR= AS= CC=" "	AR= AS= CC=\$(CONFIG_CROSS_COMPILER_PATH)/mipsel-linux-gcc CFLAGS=\"$CFLAGS\" CROSS= CROSS_COMPILE= CXX= HOSTCC= LD=\$(CONFIG_CROSS_COMPILER_PATH)/mipsel-linux-ld OBJCOPY= RANLIB= STRIP= GCC_DIR= ./configure \$(CONFOPTS)"
fi

if [ "$1" = "make" ] || [ "$1" = "clean" ] || [ "$1" = "all" ]; then
	make KERNEL_SOURCE=${LINUX_DIR} CROSS=${CROSS_COMPILE} CONFIG_CROSS_COMPILER_PATH=${CROSS_BIN} CONFIG_LIB_LIBUPNP_FORCE=y CONFIG_LIB_LIBPTHREAD_FORCE=y CONFIG_LIB_RT_FORCE=y clean 
	if [ $? = 1 ]; then exit 1; fi
fi

if [ "$1" = "make" ] || [ "$1" = "all" ]; then
	make KERNEL_SOURCE=${LINUX_DIR} CROSS=${CROSS_COMPILE} CONFIG_CROSS_COMPILER_PATH=${CROSS_BIN} CONFIG_LIB_LIBUPNP_FORCE=y CONFIG_LIB_LIBPTHREAD_FORCE=y CONFIG_LIB_RT_FORCE=y
	if [ $? = 1 ]; then exit 1; fi
fi

if [ "$1" = "install" ] || [ "$1" = "all" ]; then
	mkdir -p ${ROMFS_DIR}/lib
	cp -f lib/libupnp-0.9.28.so ${ROMFS_DIR}/lib/libupnp.so.0
	cp -f lib/libixml-0.9.28.so ${ROMFS_DIR}/lib/libixml.so.0
	cp -f lib/libpthread-0.9.28.so ${ROMFS_DIR}/lib/libpthread.so.0
	cp -f lib/libthreadutil-0.9.28.so ${ROMFS_DIR}/lib/libthreadutil.so.0
	cp -f lib/librt-0.9.28.so ${ROMFS_DIR}/lib/librt.so.0
fi

# From Ralink SDK 3.2.0.0
#
# PCI UPnP Configure
#    M libupnp/upnp/src/genlib/net/http/httpparser.c
#    M libupnp/upnp/src/inc/httpparser.h
#    M libupnp/upnp/src/ssdp/ssdp_device.c
#    M libupnp/upnp/src/ssdp/ssdp_server.c
