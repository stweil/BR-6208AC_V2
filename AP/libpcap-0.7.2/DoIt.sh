#!/bin/sh
. ../../define/PATH
if [ "$1" = "make" ] && [ -f libpcap.a ]; then exit; fi
make clean
if [ "$1" = "clean" ]; then exit; fi
CC=${CROSS_COMPILE}gcc LD=${CROSS_COMPILE}ld STRIP=${CROSS_COMPILE}strip OBJDUMP=${CROSS_COMPILE}objdump ac_cv_linux_vers=linux ./configure --build=i686 --host=mips-linux --with-pcap=linux
if [ "$1" = "configure" ]; then exit; fi
make
