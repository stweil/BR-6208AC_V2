#!/bin/sh
if [ "$1" != "configure" ] && [ "$1" != "clean" ] && [ "$1" != "make" ] && [ "$1" != "install" ] && [ "$1" != "all" ]; then
echo "$0 [ configure | clean | make | install | all ]"
exit 1
fi
. ../../define/PATH
. ../../define/FUNCTION_SCRIPT
. ${TOOLS_DIR}/script_tools.sh
PATH=${CROSS_BIN}:$PATH
if [ "$1" = "make" ] || [ "$1" = "configure" ] || [ "$1" = "all" ]; then
if [ "$_MODEL_" = "Customer" ]; then
echo ".config-busybox-BR6477AC"
cp .config-busybox-BR6477AC .config
fi
SEARCH_AND_REPLACE .config "^CONFIG_CROSS_COMPILER_PREFIX=" "CONFIG_CROSS_COMPILER_PREFIX=\"${CROSS_COMPILE}\""
SEARCH_AND_REPLACE .config "^CONFIG_FEATURE_CROND_DIR=" "CONFIG_FEATURE_CROND_DIR=\"/var/cron\""
SEARCH_AND_REPLACE .config "^CONFIG_PREFIX=" "CONFIG_PREFIX=\"${ROMFS_DIR}\""
if [ "$_WanAutoDetect_" ] || [ "$_IQSETUP_" ]; then
SEARCH_AND_REPLACE .config "CONFIG_HTTPD" "CONFIG_HTTPD=y"
SEARCH_AND_REPLACE .config "CONFIG_FEATURE_HTTPD_CGI" "CONFIG_FEATURE_HTTPD_CGI=y"
fi
if [ "$_IQSETUP_" ];then
SEARCH_AND_REPLACE .config "CONFIG_PING6" "CONFIG_PING6=n"
SEARCH_AND_REPLACE .config "CONFIG_SHA1SUM" "CONFIG_SHA1SUM=n"
fi
if [ "$_TELNET_REMOTE_CONTROL_" ]; then
SEARCH_AND_REPLACE .config "CONFIG_TELNETD" "CONFIG_TELNETD=y"
SEARCH_AND_REPLACE .config "CONFIG_FEATURE_TELNETD_STANDALONE" "CONFIG_FEATURE_TELNETD_STANDALONE=y"
SEARCH_AND_REPLACE .config "CONFIG_FEATURE_DEVPTS" "# CONFIG_FEATURE_DEVPTS is not set"
SEARCH_AND_REPLACE .config "CONFIG_CHPASSWD" "CONFIG_CHPASSWD=y"
fi
if [ "$_OPENVPN_" ]; then
SEARCH_AND_REPLACE .config "# CONFIG_TFTP is not set" "CONFIG_TFTP=y"
SEARCH_AND_REPLACE .config "# CONFIG_FEATURE_TFTP_PUT is not set" "CONFIG_FEATURE_TFTP_PUT=y"
SEARCH_AND_REPLACE .config "# CONFIG_UNZIP is not set" "CONFIG_UNZIP=y"
SEARCH_AND_REPLACE .config "# CONFIG_TAR is not set" "CONFIG_TAR=y"
SEARCH_AND_REPLACE .config "# CONFIG_FEATURE_TAR_CREATE is not set" "CONFIG_FEATURE_TAR_CREATE=y"
SEARCH_AND_REPLACE .config "# CONFIG_FEATURE_TAR_LONG_OPTIONS is not set" "CONFIG_FEATURE_TAR_LONG_OPTIONS=y"
fi
if [ "$_WIFI_ROMAING_" ]; then
SEARCH_AND_REPLACE .config "# CONFIG_UDPSVD is not set" "CONFIG_UDPSVD=y"
fi
fi
if [ "$1" = "clean" ] || [ "$1" = "all" ]; then
make clean
if [ $? = 1 ]; then exit 1; fi
fi
if [ "$1" = "make" ] || [ "$1" = "all" ]; then
make
if [ $? = 1 ]; then exit 1; fi
fi
if [ "$1" = "install" ] || [ "$1" = "all" ]; then
make install
if [ $? = 1 ]; then exit 1; fi
ln -sf bin/busybox ${ROMFS_DIR}/init	
mkdir -p ${ROMFS_DIR}/usr/share
mkdir -p ${ROMFS_DIR}/etc/udhcpc
cp ${BUSYBOX_DIR}/mips-scripts/* ${ROMFS_DIR}/etc/udhcpc
mkdir -p ${ROMFS_DIR}/var/lib
mkdir -p ${ROMFS_DIR}/var/lib/misc
fi
