#!/bin/sh
set -e
. ../../define/PATH

if [ -f Makefile -a "$1" = "clean" ]; then
	make distclean
	rm -f ${APPLICATIONS_DIR}/openvpn-2.3.2/libs4Compile/lib/liblzo2.a
fi
if [ -f ${APPLICATIONS_DIR}/openvpn-2.3.2/libs4Compile/lib/liblzo2.a ]; then
	exit
fi
if [ "$1" = "clean" ]; then exit; fi
CC=${CROSS_COMPILE}gcc RANLIB=${CROSS_COMPILE}ranlib STRIP=${CROSS_COMPILE}strip CFLAGS="-Os" ./configure --host=mips-linux --prefix=${APPLICATIONS_DIR}/openvpn-2.3.2/libs4Compile
#if [ "$1" = "configure" ]; then exit; fi
make
make install
