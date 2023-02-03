#!/bin/sh
. ../../define/PATH
. ../../define/FUNCTION_SCRIPT

if [ "$1" = "install" ] || [ "$1" = "all" ]; then
	mkdir -p ${ROMFS_DIR}/bin
	rm -rf wlan_scan
	if [ "$_EW7478ACR_" = "y" ]; then
		cp -f wlan_scan_Romaing wlan_scan
	else
		cp -f wlan_scan_ok wlan_scan
	fi
	cp -f wlan_scan ${ROMFS_DIR}/bin
	if [ $? = 1 ]; then exit 1; fi
fi


