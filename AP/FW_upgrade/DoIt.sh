#!/bin/sh
. ../../define/PATH
if [ "$1" = "install" ] || [ "$1" = "all" ]; then
	mkdir -p ${ROMFS_DIR}/bin
	cp -f fw_upgrade ${ROMFS_DIR}/bin
	if [ $? = 1 ]; then exit 1; fi
fi

