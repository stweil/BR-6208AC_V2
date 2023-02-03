#!/bin/sh -e
if [ "$1" = "make" ] && [ -f resolveip ]; then exit; fi
. ../../define/PATH
rm -rf resolveip
if [ "$1" = "clean" ]; then exit; fi
${CROSS_COMPILE}gcc -Os -Wall -o resolveip resolveip.c
${CROSS_COMPILE}strip resolveip
