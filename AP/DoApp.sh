#!/bin/sh
if [ "$1" != "configure" ] && [ "$1" != "clean" ] && [ "$1" != "make" ] && [ "$1" != "install" ] && [ "$1" != "all" ]; then
echo "$0 [ configure | clean | make | install | all ]"
exit 1
fi
. ../define/PATH
. ../define/FUNCTION_SCRIPT
. ../define/DO_LIST
. ../tools/script_tools.sh
if [ "${1}" = "configure" ] || [ "${1}" = "all" ]; then
for DIR in ${DO_LIST}; do
if [ ! -d $DIR ] || [ ! -f $DIR/DoIt.sh ]; then
continue
fi
cd $DIR
DRAW_LINE 80 "Configuring $DIR" 33
./DoIt.sh configure
if [ $? = 1 ]; then DRAW_LINE 80 "Configure $DIR fail" 31; exit 1; fi
DRAW_LINE 80 "Configure $DIR success" 32
cd ..
done
fi
if [ "${1}" = "clean" ] || [ "${1}" = "all" ]; then
for DIR in ${DO_LIST}; do
if [ ! -d $DIR ] || [ ! -f $DIR/DoIt.sh ]; then
continue
fi
cd $DIR
DRAW_LINE 80 "Cleaning $DIR" 33
./DoIt.sh clean
if [ $? = 1 ]; then DRAW_LINE 80 "Clean $DIR fail" 31; exit 1; fi
DRAW_LINE 80 "Clean $DIR success" 32
cd ..
done
fi
if [ "${1}" = "make" ] || [ "${1}" = "all" ]; then
for DIR in ${DO_LIST}; do
if [ ! -d $DIR ] || [ ! -f $DIR/DoIt.sh ]; then
continue
fi
cd $DIR
DRAW_LINE 80 "Compiling $DIR" 33
./DoIt.sh make
if [ $? = 1 ]; then DRAW_LINE 80 "Compile $DIR fail" 31; exit 1; fi
DRAW_LINE 80 "Compile $DIR success" 32
cd ..
done
fi
if [ "${1}" = "install" ] || [ "${1}" = "all" ]; then
DRAW_LINE 80 "Collecting Applications" 33
rm -f ${IMAGE_DIR}/appimg
rm -rf ${ROMFS_DIR}
for DIR in ${DO_LIST}; do
if [ ! -d $DIR ] || [ ! -f $DIR/DoIt.sh ]; then
continue
fi
cd $DIR
echo -n "Installing $DIR"
DRAW "`expr 80 - length \"Installing $DIR\" - 8`" "."
./DoIt.sh install
if [ $? = 1 ]; then
echo -e "[\033[31;1mFAILED\033[0m]"
exit 1
fi
echo -e "[\033[32;1m  OK  \033[0m]"
cd ..
done
mkdir -p ${ROMFS_DIR}
mkdir -p ${ROMFS_DIR}/dev
mkdir -p ${ROMFS_DIR}/bin
mkdir -p ${ROMFS_DIR}/sbin
mkdir -p ${ROMFS_DIR}/usr
mkdir -p ${ROMFS_DIR}/etc
mkdir -p ${ROMFS_DIR}/etc_ro
mkdir -p ${ROMFS_DIR}/var
mkdir -p ${ROMFS_DIR}/var/cron
mkdir -p ${ROMFS_DIR}/proc
mkdir -p ${ROMFS_DIR}/tmp
mkdir -p ${ROMFS_DIR}/sys
mkdir -p ${ROMFS_DIR}/bin/ACCOUNT_SAMBA
mkdir -p ${ROMFS_DIR}/lib
cp ${CROSS_LIB}/libuClibc-0.9.28.so ${ROMFS_DIR}/lib/libc.so.0
cp ${CROSS_LIB}/ld-uClibc-0.9.28.so ${ROMFS_DIR}/lib/ld-uClibc.so.0
cp ${CROSS_LIB}/libcrypt-0.9.28.so ${ROMFS_DIR}/lib/libcrypt.so.0
cp ${CROSS_LIB}/libdl-0.9.28.so ${ROMFS_DIR}/lib/libdl.so.0
cp ${CROSS_LIB}/libutil-0.9.28.so ${ROMFS_DIR}/lib/libutil.so.0
cp ${CROSS_LIB}/libresolv-0.9.28.so ${ROMFS_DIR}/lib/libresolv.so.0 #for rdisc
cp ${CROSS_LIB}/libnsl-0.9.28.so ${ROMFS_DIR}/lib/libnsl.so.0 #for appletalk
cp ${CROSS_LIB}/libm-0.9.28.so ${ROMFS_DIR}/lib/libm.so.0
cp -R -p ${APPLICATIONS_DIR}/etc.adm/* ${ROMFS_DIR}/etc
rm ${ROMFS_DIR}/etc/profile.ap -f
rm ${ROMFS_DIR}/etc/init.d/rcS.ath -f
rm -rf ${ROMFS_DIR}/bin/ipv6*
cp ${APPLICATIONS_DIR}/axTLS/_stage/axhttpd ${ROMFS_DIR}/bin
cp ${APPLICATIONS_DIR}/wireless_tools.29/iwconfig ${ROMFS_DIR}/bin
if [ "$_AUTO_FW_UPGRADE_" = "y" -o "$_OPENVPN_" = "y" ]; then
cp -f ${APPLICATIONS_DIR}/curl-7.34.0/install/bin/curl ${ROMFS_DIR}/bin/
cp -rf ${APPLICATIONS_DIR}/polarssl-1.2.12/build/lib/libpolarssl.so ${ROMFS_DIR}/lib/libpolarssl.so.5
fi
if [ "$_OPENVPN_" = "y" ]; then
cp -rf ${APPLICATIONS_DIR}/libz-1.2.8/libz.so.1.2.8 ${ROMFS_DIR}/lib/libz.so.1
cp ${APPLICATIONS_DIR}/openvpn-2.3.2/src/openvpn/openvpn ${ROMFS_DIR}/bin
mkdir -p ${ROMFS_DIR}/etc/openvpn
mkdir -p ${ROMFS_DIR}/etc/openvpn/totalKeys
cp -rf ${APPLICATIONS_DIR}/openvpn-keys/CurrentKeys/* ${ROMFS_DIR}/etc/openvpn/totalKeys
fi
if [ "$_IGMP_PROXY_" = "y" ]; then
cp -Rf ${APPLICATIONS_DIR}/igmpproxy_mtk/src/igmpproxy ${ROMFS_DIR}/bin
cp -Rf ${APPLICATIONS_DIR}/igmpproxy_mtk/src/igmpproxy.sh ${ROMFS_DIR}/bin
fi
if [ "$_DDNS_NOIP_" = "y" ]; then    
cp -Rf ${APPLICATIONS_DIR}/noip-2.1.9-1/noip2 ${ROMFS_DIR}/bin 
fi
if [ "$_EZ_QOS_" = "y" ]; then    
cp -f ${LINUX_DIR}/net/netfilter/xt_layer7.ko ${ROMFS_DIR}/bin/xt_layer7.ko
fi
if [ "$_MODEL_" = "EW7478AC" ];then
if [ "$_GBAND_SKU_ENABLE_" = "y" -o "$_ABAND_SKU_ENABLE_" = "y" ]; then
cp -f ${ROOT_DIR}/kernel_module/wireless/mt7612_ap/3.0.3.2/rlt_wifi/conf/7478-SingleSKU_CE.dat ${ROMFS_DIR}/bin #CE 歐
cp -f ${ROOT_DIR}/kernel_module/wireless/mt7612_ap/3.0.3.2/rlt_wifi/conf/7478-SingleSKU_FCC.dat ${ROMFS_DIR}/bin #FCC 美
cp -f ${ROOT_DIR}/kernel_module/wireless/mt7612_ap/3.0.3.2/rlt_wifi/conf/7478-SingleSKU_AU.dat ${ROMFS_DIR}/bin #AU 澳
fi
fi
cp ${APPLICATIONS_DIR}/wps_scan/wps_scan ${ROMFS_DIR}/bin
mkdir -p ${ROMFS_DIR}/etc_ro/lan
mkdir -p ${ROMFS_DIR}/etc/Wireless/RT2860
mkdir -p ${ROMFS_DIR}/etc/Wireless/RT2860STA
mkdir -p ${ROMFS_DIR}/etc/Wireless/iNIC
mkdir -p ${ROMFS_DIR}/etc_ro/lan
mkdir -p ${ROMFS_DIR}/etc_ro/wlan
mkdir -p ${ROMFS_DIR}/etc_ro/Wireless
mkdir -p ${ROMFS_DIR}/etc_ro/Wireless/RT2860AP
mkdir -p ${ROMFS_DIR}/etc_ro/Wireless/iNIC
if [ "$_RFCOMB_" = "3" ]; then
cp -f ${ROOT_DIR}/kernel_module/rt2860v2_ap/rt2860v2_ap.ko ${ROMFS_DIR}/bin	# 2.4G driver
cp -f ${ROOT_DIR}/kernel_module/rlt_wifi_ap/rlt_wifi.ko ${ROMFS_DIR}/bin	  # 5G driver
cp -f ${APPLICATIONS_DIR}/wireless_driver/configure/7478-2.4g-ee-FCC-0908.bin ${ROMFS_DIR}/etc_ro/wlan/MT7620_AP_2T2R-4L.BIN
cp -f ${APPLICATIONS_DIR}/wireless_driver/configure/7478-5g-ee-FCC-0907.bin ${ROMFS_DIR}/etc_ro/wlan/MT7612E_EEPROM.bin
else
cp -f ${ROOT_DIR}/kernel_module/rt2860v2_ap/rt2860v2_ap.ko ${ROMFS_DIR}/bin	# 2.4G driver
cp -f ${ROOT_DIR}/kernel_module/mt7610_ap/MT7610_ap.ko	${ROMFS_DIR}/bin	  # 5G driver
cp -f ${APPLICATIONS_DIR}/wireless_driver/configure/BR6278AC_2G_for_MTK_FW_20160715.bin ${ROMFS_DIR}/etc_ro/wlan/MT7620_AP_2T2R-4L.BIN
cp -f ${APPLICATIONS_DIR}/wireless_driver/configure/BR6278AC_5G_for_MTK_FW_20160715.bin ${ROMFS_DIR}/etc_ro/wlan/MT7610E-V10-eeprom-20121128-1-FEM-1ANT.bin
fi
if [ "$_AUTO_CHAN_SELECT_" = "y" ]; then
cp -rf ${APPLICATIONS_DIR}/wlan_scan/wlan_scan ${ROMFS_DIR}/bin/wlan_scan
fi
if [ "$_WIFI_ROMAING_" = "y" ]; then
cp -rf ${APPLICATIONS_DIR}/FW_upgrade/fw_upgrade ${ROMFS_DIR}/bin/fw_upgrade
fi
if [ "$_TIMESERVER_" = "y" ]; then
cp ${APPLICATIONS_DIR}/clockspeed-0.62/sntpclock ${ROMFS_DIR}/bin
fi
cp ${APPLICATIONS_DIR}/resolveip/resolveip ${ROMFS_DIR}/bin
cp ${APPLICATIONS_DIR}/Edimax_Cloud/edx_cloud ${ROMFS_DIR}/bin
if [ "$_WIFI_REPEATER_" = "y" ]; then
cp -f ${APPLICATIONS_DIR}/mode_sw/mode_sw ${ROMFS_DIR}/bin/mode_sw
fi
if [ "$_LOCAL_SEARCH_" = "y" ]; then  
${CROSS_COMPILE}strip ${APPLICATIONS_DIR}/jansson-2.7/_install/lib/libjansson.so.4.7.0 -o ${ROMFS_DIR}/lib/libjansson.so.4
fi
cp ${APPLICATIONS_DIR}/fping-2.4b2_interface/fping ${ROMFS_DIR}/bin
cp ${IMAGE_DIR}/config.bin ${ROMFS_DIR}/etc/config.bin
echo ${_VERSION_} > ${ROMFS_DIR}/etc/version
echo ${_DATE_} >  ${ROMFS_DIR}/etc/compiler_date
cd ${ROOT_DIR}/
svn info > ${ROMFS_DIR}/etc/svn_info
cat ${LINUX_DIR}/.config > ${ROMFS_DIR}/etc/SDK_CONFIG
cp -R -p ${APPLICATIONS_DIR}/var/* ${ROMFS_DIR}/var
cp -Rf ${ROMFS_DIR}/dev ${ROMFS_DIR}/dev.tmp
mkdir -p ${ROMFS_DIR}/etc.tmp
mv ${ROMFS_DIR}/etc/* ${ROMFS_DIR}/etc.tmp
mv ${ROMFS_DIR}/etc.tmp/profile ${ROMFS_DIR}/etc
mv ${ROMFS_DIR}/etc.tmp/inittab ${ROMFS_DIR}/etc
find ${ROMFS_DIR} -name .svn | xargs -i rm -rf {}
chmod 777 ${ROMFS_DIR}/bin/*.*
if [ ! -f ${TOOLS_DIR}/clean-space/clean-space ]; then
cd ${TOOLS_DIR}/clean-space
gcc -o clean-space clean-space.c
fi
${TOOLS_DIR}/clean-space/clean-space ${ROMFS_DIR}
find ${ROMFS_DIR}/* | xargs -i file {}  | grep "strip" | cut -f1 -d":" | xargs -r ${CROSS_COMPILE}strip -R .comment -R .note -g --strip-unneeded;
cp ${APPLICATIONS_DIR}/script/*.sh ${ROMFS_DIR}/bin
cd ${APPLICATIONS_DIR}
DRAW_LINE 80 "Compressing appimg" 33
${TOOLS_DIR}/squashfs4.2/squashfs-tools/mksquashfs ${ROMFS_DIR} ${IMAGE_DIR}/appimg
if [ $? = 0 ]; then
DRAW_LINE 80 "Compress appimg success" 33
else
DRAW_LINE 80 "Compress appimg fail" 35
exit 1
fi
size -A -x --target=binary ${IMAGE_DIR}/appimg
chmod +r ${IMAGE_DIR}/appimg
ls -l ${IMAGE_DIR}/appimg
fi
