#!/bin/sh
if [ "$1" != "configure" ] && [ "$1" != "clean" ] && [ "$1" != "make" ] && [ "$1" != "install" ] && [ "$1" != "all" ]; then
	echo "$0 [ configure | clean | make | install | all ]"
fi

set -e 

app=(
'Jensen'
)
#'wsc_upnp'
app_num=`echo ${#app[*]}`

. ../../define/PATH

PATH=${CROSS_BIN}:$PATH

#LIBUPNP_SRC_PATH=${APPLICATIONS_DIR}/lib/libupnp
#LIB_PATH=${APPLICATIONS_DIR}/lib/lib

#ver=`echo ${LINUX_DIR} | cut -f 2 -d '_'`

if [ "$1" = "configure" ] || [ "$1" = "all" ]; then
	:
	if [ $? = 1 ]; then exit 1; fi
fi

if [ "$1" = "clean" ] || [ "$1" = "all" ]; then
	for (( n = 0; $n < $app_num; n = n + 1 ))do
		make -C ${app[$n]}/ clean
		if [ $? = 1 ]; then exit 1; fi
	done
fi

if [ "$1" = "make" ] || [ "$1" = "all" ]; then
	for (( n = 0; $n < $app_num; n = n + 1))do
		make -C ${app[$n]}/ CC=${CROSS_COMPILE}gcc ROOTDIR=${ROOT_DIR} LINUXDIR=${LINUX_DIR} LDFLAGS=""
		if [ $? = 1 ]; then exit 1; fi
	done
fi

if [ "$1" = "install" ] || [ "$1" = "all" ]; then
	for (( n = 0; $n < $app_num; n = n + 1))do
		cp -f ${app[$n]}/agent ${ROMFS_DIR}/bin
		if [ $? = 1 ]; then exit 1; fi
	done
fi

