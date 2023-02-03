#!/bin/sh
set -e
. ../../define/PATH

if [ -f Makefile -a "$1" = "clean" ]; then
	make distclean
	rm -f ${APPLICATIONS_DIR}/openvpn-2.3.2/libs4Compile/lib/libz.a
fi
if [ -f ${APPLICATIONS_DIR}/openvpn-2.3.2/libs4Compile/lib/libz.a ]; then
	exit
fi
if [ "$1" = "clean" ]; then exit; fi
CC=${CROSS_COMPILE}gcc RANLIB=${CROSS_COMPILE}ranlib STRIP=${CROSS_COMPILE}strip CFLAGS="-Os" ./configure --prefix=${APPLICATIONS_DIR}/openvpn-2.3.2/libs4Compile
#if [ "$1" = "configure" ]; then exit; fi
make
make install
