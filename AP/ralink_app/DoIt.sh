#!/bin/bash
if [ "$1" != "configure" ] && [ "$1" != "clean" ] && [ "$1" != "make" ] && [ "$1" != "install" ] && [ "$1" != "all" ]; then
	echo "$0 [ configure | clean | make | install | all ]"
fi

set -e 
. ../../define/PATH
. ../../define/FUNCTION_SCRIPT

ver=`echo ${LINUX_DIR} | cut -f 3 -d '_'` #patrick
if [ "$_MODEL_" = "Customer" ]; then
app=(
'ated'
'mii_mgr'
'reg'
'switch'
'switch_7530'
'wsc_upnp'
)
else
app=(
'ated'
'mii_mgr'
'reg'
'switch'
'wsc_upnp'
)
fi

#'wsc_upnp'
app_num=`echo ${#app[*]}`


PATH=${CROSS_BIN}:$PATH

LIBUPNP_SRC_PATH=${APPLICATIONS_DIR}/lib/libupnp
LIB_PATH=${APPLICATIONS_DIR}/lib/lib



if [ "$1" = "configure" ] || [ "$1" = "all" ]; then
	:
	if [ $? = 1 ]; then exit 1; fi
fi

if [ "$1" = "clean" ] || [ "$1" = "all" ]; then
	for (( n = 0; $n < $app_num; n = n + 1 ))do
		make -C $ver/${app[$n]}/ clean
		if [ $? = 1 ]; then exit 1; fi
	done
fi

if [ "$1" = "make" ] || [ "$1" = "all" ]; then
	for (( n = 0; $n < $app_num; n = n + 1))do
		if [ ${app[$n]} = "wsc_upnp" ];then
		make -C $ver/${app[$n]}/\
	        CROSS_COMPILE=${CROSS_COMPILE}\
        	WIRELESS_H_INCLUDE_PATH=${LINUX_DIR}/include\
	        LIBUPNP_SRC_PATH=${APPLICATIONS_DIR}/lib/libupnp\
        	LIB_PATH=${APPLICATIONS_DIR}/lib/lib

		else
		make -C $ver/${app[$n]}/ CC=${CROSS_COMPILE}gcc ROOTDIR=${ROOT_DIR} LINUXDIR=${LINUX_DIR} LDFLAGS="" CFLAGS="-I${LINUX_DIR}/drivers/net/raeth -I${LINUX_DIR}/drivers/net/rt_rdm -I${LINUX_DIR}/include"
		fi
		if [ $? = 1 ]; then exit 1; fi
	done
fi

if [ "$1" = "install" ] || [ "$1" = "all" ]; then
	for (( n = 0; $n < $app_num; n = n + 1))do

		if [ ${app[$n]} = "wsc_upnp" ];then
			cp -f $ver/${app[$n]}/wscd ${ROMFS_DIR}/bin
			mkdir -p ${ROMFS_DIR}/etc/xml
			cp -f $ver/${app[$n]}/xml/* ${ROMFS_DIR}/etc/xml
			if [ $? = 1 ]; then exit 1; fi
		else
			cp -f $ver/${app[$n]}/${app[$n]} ${ROMFS_DIR}/bin
			if [ $? = 1 ]; then exit 1; fi
		fi

	done
fi

