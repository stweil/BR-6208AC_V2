#!/bin/sh
. ../../define/PATH
export PATH=$CROSSDIR:$PATH
rm -f sntpclock load conf-cc systype conf-ld make-makelib make-compile auto-ccld.sh make-load find-systype compile
rm -f *.o
rm -f *.a
if [ "$1" = "clean" ]; then exit; fi
make CC=${CROSS_COMPILE}gcc
${CROSS_COMPILE}strip sntpclock
