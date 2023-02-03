#!/bin/sh
. ../../define/PATH
if [ "$1" = "make" ] && [ -f fping ]; then exit; fi
make clean
if [ "$1" = "clean" ]; then exit; fi
CC=${CROSS_COMPILE}gcc CFLAGS="-Os" ./configure --host=mips-linux
if [ "$1" = "configure" ]; then exit; fi
make
${CROSS_COMPILE}strip fping
