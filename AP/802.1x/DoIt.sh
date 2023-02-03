#!/bin/sh
if [ "$1" != "configure" ] && [ "$1" != "clean" ] && [ "$1" != "make" ] && [ "$1" != "install" ] && [ "$1" != "all" ]; then
	echo "$0 [ configure | clean | make | install | all ]"
	exit 1
fi

. ../../define/PATH
PATH=${CROSS_BIN}:$PATH

. ../../define/FUNCTION_SCRIPT
if [ "$1" = "configure" ] || [ "$1" = "all" ]; then
	:
fi

if [ "$1" = "clean" ] || [ "$1" = "all" ]; then
	make clean
	if [ $? = 1 ]; then exit 1; fi
fi

if [ "$1" = "make" ] || [ "$1" = "all" ]; then
	make CROSS_COMPILE=${CROSS_COMPILE} RFCODE=${_RFCODE_}
	if [ $? = 1 ]; then exit 1; fi

	${CROSS_COMPILE}strip rt2860apd
	${CROSS_COMPILE}strip rt2860apd -o rtinicapd
	if [ $? = 1 ]; then exit 1; fi
fi

if [ "$1" = "install" ] || [ "$1" = "all" ]; then
	mkdir -p ${ROMFS_DIR}/bin
	cp -f rt2860apd ${ROMFS_DIR}/bin
	cp -f rtinicapd ${ROMFS_DIR}/bin
	if [ $? = 1 ]; then exit 1; fi
fi

cd ..
