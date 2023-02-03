#!/bin/sh
if [ "$1" != "configure" ] && [ "$1" != "clean" ] && [ "$1" != "make" ] && [ "$1" != "install" ] && [ "$1" != "all" ]; then
	echo "$0 [ configure | clean | make | install | all ]"
	exit 1
fi

. ../../define/PATH
. ../../tools/script_tools.sh
PATH=${CROSS_BIN}:$PATH
export KDIR=$LINUX_DIR

if [ "$1" = "configure" ] || [ "$1" = "make" ] || [ "$1" = "all" ]; then
	cd pppd/plugins/pptp_plugin

	AR=${CROSS_COMPILE}ar\
	CC=${CROSS_COMPILE}gcc\
	STRIP=${CROSS_COMPILE}strip\
	RANLIB=${CROSS_COMPILE}ranlib\
	./configure --host=mipsel-linux

	SEARCH_AND_REPLACE Makefile "^CFLAGS = " "CFLAGS = -g -O2 -I. -I${LINUX_DIR}/include"
	SEARCH_AND_REPLACE src/Makefile "^CFLAGS = " "CFLAGS = -g -O2 -I. -I${LINUX_DIR}/include"
	SEARCH_AND_REPLACE config.h "^#define PPPD_VERSION" "#define PPPD_VERSION \"2.4.2b1\""
	cd ../../..
fi

if [ "$1" = "clean" ] || [ "$1" = "all" ]; then
	cd pppd/plugins/openl2tp-1.6
	make clean
	if [ $? != 0 ]; then exit 1; fi
	cd ../../..

	cd pppd/plugins/pptp_plugin
	make clean
	if [ $? != 0 ]; then exit 1; fi
	cd ../../..

	cd pppd/plugins/rp-pppoe
	make clean
	if [ $? != 0 ]; then exit 1; fi
	cd ../../..
	
	cd pppd
	make clean
	if [ $? != 0 ]; then exit 1; fi
	cd ..
	
	cd chat
	make clean
	if [ $? != 0 ]; then exit 1; fi
	cd ..
fi

if [ "$1" = "make" ] || [ "$1" = "all" ]; then
	
	cd pppd/plugins/openl2tp-1.6/pppd
	
	make CC=${CROSS_COMPILE}gcc CFLAGS.optimize="-O" L2TP_KERNELDIR=${LINUX_DIR} KERNEL_BLDDIR=${LINUX_DIR} KERNEL_SRCDIR=${LINUX_DIR} INCLUDE_DIR=${APPLICATIONS_DIR}/ppp-2.4.2/pppd/plugins/pptp_plugin/src

	cd ../../../..

	cd pppd/plugins/pptp_plugin
	make
	if [ $? != 0 ]; then exit 1; fi
	${CROSS_COMPILE}strip src/.libs/pptp.so.0.0.0
	if [ $? != 0 ]; then exit 1; fi
	cd ../../..

	cd pppd/plugins/rp-pppoe
	make
	if [ $? != 0 ]; then exit 1; fi
	${CROSS_COMPILE}strip rp-pppoe.so
	if [ $? != 0 ]; then exit 1; fi
	cd ../../..

	cd pppd
	make
	if [ $? != 0 ]; then exit 1; fi
	cd ..
	
	cd chat
	make
	if [ $? != 0 ]; then exit 1; fi
	cd ..
fi

if [ "$1" = "install" ] || [ "$1" = "all" ]; then
	mkdir -p ${ROMFS_DIR}/usr/sbin
	mkdir -p ${ROMFS_DIR}/sbin
	mkdir -p ${ROMFS_DIR}/bin
	mkdir -p ${ROMFS_DIR}/lib
	ln -s /bin/pppd ${ROMFS_DIR}/usr/sbin/pppd
	#cp -f chat/chat ${ROMFS_DIR}/sbin
	cp -f pppd/pppd ${ROMFS_DIR}/bin
	cp -f pppd/plugins/pptp_plugin/src/.libs/pptp.so.0.0.0 ${ROMFS_DIR}/lib/pptp.so
	cp -f pppd/plugins/rp-pppoe/rp-pppoe.so ${ROMFS_DIR}/lib
	cp -f pppd/plugins/openl2tp-1.6/pppd/pppol2tp.so ${ROMFS_DIR}/lib
	
	#cp -f pppd/plugins/rp-pppoe/libplugin.a ${ROMFS_DIR}/lib

	mkdir -p ${ROMFS_DIR}/etc/ppp
	touch ${ROMFS_DIR}/etc/ppp/pap-secrets
	chmod 600 ${ROMFS_DIR}/etc/ppp/pap-secrets
	touch ${ROMFS_DIR}/etc/ppp/chap-secrets
	chmod 600 ${ROMFS_DIR}/etc/ppp/chap-secrets
fi
