#!/bin/sh
. ../../define/PATH
if [ "$1" = "make" ]; then exit; fi
install -d ${ROMFS_DIR}/usr/sbin
install -m 755 airfores ${ROMFS_DIR}/usr/sbin/airfores
install -m 755 gddb ${ROMFS_DIR}/usr/sbin/gddb
ln -sf ./gddb ${ROMFS_DIR}/usr/sbin/gddbc
install -m 755 gdwebd ${ROMFS_DIR}/usr/sbin/gdwebd
install -d ${ROMFS_DIR}/lib
install -m 755 libairfores.so ${ROMFS_DIR}/lib
install -m 755 libgdapi.so ${ROMFS_DIR}/lib
cp scripts/* ${ROMFS_DIR}/bin
