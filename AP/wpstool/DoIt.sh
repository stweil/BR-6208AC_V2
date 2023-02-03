#!/bin/sh
. ../../define/PATH
. ../../define/FUNCTION_SCRIPT

if [ "$1" = "install" ] || [ "$1" = "all" ]; then
	mkdir -p ${ROMFS_DIR}/bin
	rm -rf wpstool
	if [ "$_EW7478ACR_" = "y" ]; then
		cp -f wpstool_Romaing wpstool
	else
		cp -f wpstool_ok wpstool
	fi
	cp -f wpstool ${ROMFS_DIR}/bin
	if [ $? = 1 ]; then exit 1; fi
fi


