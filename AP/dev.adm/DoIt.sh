#!/bin/sh
if [ "$1" != "configure" ] && [ "$1" != "clean" ] && [ "$1" != "make" ] && [ "$1" != "install" ] && [ "$1" != "all" ]; then
	echo "$0 [ configure | clean | make | install | all ]"
	exit 1
fi

. ../../define/PATH
. ../../define/FUNCTION_SCRIPT
PATH=${CROSS_BIN}:$PATH

if [ "$1" = "configure" ] || [ "$1" = "all" ]; then
	:
fi

if [ "$1" = "clean" ] || [ "$1" = "all" ]; then
	:
fi

if [ "$1" = "make" ] || [ "$1" = "all" ]; then
	:
fi

if [ "$1" = "install" ] || [ "$1" = "all" ]; then
	mkdir -p ${ROMFS_DIR}/dev
	mknod -m666 ${ROMFS_DIR}/dev/ram0       c   1  0
	mknod -m666 ${ROMFS_DIR}/dev/mem        c   1  1
	mknod -m666 ${ROMFS_DIR}/dev/kmem       c   1  2 	
	mknod -m666 ${ROMFS_DIR}/dev/null       c   1  3
	mknod -m666 ${ROMFS_DIR}/dev/zero       c   1  5
	mknod -m666 ${ROMFS_DIR}/dev/random     c   1  8
	mknod -m666 ${ROMFS_DIR}/dev/urandom    c   1  9
	mknod -m666 ${ROMFS_DIR}/dev/ptyp0      c   2  0
	mknod -m666 ${ROMFS_DIR}/dev/ptyp1      c   2  1
	mknod -m666 ${ROMFS_DIR}/dev/ptyp2      c   2  2
	mknod -m666 ${ROMFS_DIR}/dev/ptyp3      c   2  3
	mknod -m666 ${ROMFS_DIR}/dev/ttyp0      c   3  0
	mknod -m666 ${ROMFS_DIR}/dev/ttyp1      c   3  1
	mknod -m666 ${ROMFS_DIR}/dev/ttyp2      c   3  2
	mknod -m666 ${ROMFS_DIR}/dev/ttyp3      c   3  3
	mknod -m666 ${ROMFS_DIR}/dev/tty0       c   4  0
	mknod -m666 ${ROMFS_DIR}/dev/tty1       c   4  1
	mknod -m666 ${ROMFS_DIR}/dev/tty2       c   4  2
	mknod -m666 ${ROMFS_DIR}/dev/ttyS0      c   4 64
	mknod -m666 ${ROMFS_DIR}/dev/ttyS1      c   4 65
	mknod -m666 ${ROMFS_DIR}/dev/ttyS2      c   4 66
	mknod -m666 ${ROMFS_DIR}/dev/tty        c   5  0
	mknod -m666 ${ROMFS_DIR}/dev/console    c   5  1
	mknod -m600 ${ROMFS_DIR}/dev/ptmx       c   5  2
	mknod -m600 ${ROMFS_DIR}/dev/sda        b   8  0
	mknod -m600 ${ROMFS_DIR}/dev/sda1       b   8  1
	mknod -m600 ${ROMFS_DIR}/dev/sda2       b   8  2
	mknod -m600 ${ROMFS_DIR}/dev/sda3       b   8  3
	mknod -m600 ${ROMFS_DIR}/dev/sda4       b   8  4
	mknod -m600 ${ROMFS_DIR}/dev/sda5       b   8  5
	mknod -m600 ${ROMFS_DIR}/dev/sda6       b   8  6
	mknod -m600 ${ROMFS_DIR}/dev/sda7       b   8  7
	mknod -m600 ${ROMFS_DIR}/dev/sda8       b   8  8
	mknod -m600 ${ROMFS_DIR}/dev/sda9       b   8  9
	mknod -m600 ${ROMFS_DIR}/dev/sda10      b   8 10
	mknod -m600 ${ROMFS_DIR}/dev/sda11      b   8 11
	mknod -m600 ${ROMFS_DIR}/dev/sda12      b   8 12
	mknod -m600 ${ROMFS_DIR}/dev/sda13      b   8 13
	mknod -m600 ${ROMFS_DIR}/dev/sda14      b   8 14
	mknod -m600 ${ROMFS_DIR}/dev/sda15      b   8 15

	mknod -m666 ${ROMFS_DIR}/dev/mtdblock0  b  31  0
	mknod -m666 ${ROMFS_DIR}/dev/mtdblock1  b  31  1
	mknod -m666 ${ROMFS_DIR}/dev/mtdblock2  b  31  2
	mknod -m666 ${ROMFS_DIR}/dev/mtdblock3  b  31  3
	mknod -m666 ${ROMFS_DIR}/dev/mtdblock4  b  31  4
	mknod -m666 ${ROMFS_DIR}/dev/mtdblock5  b  31  5
	if [ "$_ROOTFS_IN_FLASH_" = "y" ];then
		mknod -m666 ${ROMFS_DIR}/dev/mtdblock6  b  31  6
		mknod -m666 ${ROMFS_DIR}/dev/mtdblock7  b  31  7
	fi	
	mknod -m666 ${ROMFS_DIR}/dev/dk0        c  63  0
	mknod -m666 ${ROMFS_DIR}/dev/dk1        c  63  1
	mknod -m666 ${ROMFS_DIR}/dev/mtd0       c  90  0
	mknod -m666 ${ROMFS_DIR}/dev/mtd0ro     c  90  1
	mknod -m666 ${ROMFS_DIR}/dev/mtd1       c  90  2
	mknod -m666 ${ROMFS_DIR}/dev/mtd1ro     c  90  3
	mknod -m666 ${ROMFS_DIR}/dev/mtd2       c  90  4
	mknod -m666 ${ROMFS_DIR}/dev/mtd2ro     c  90  5
	mknod -m666 ${ROMFS_DIR}/dev/mtd3       c  90  6
	mknod -m666 ${ROMFS_DIR}/dev/mtd3ro     c  90  7
	mknod -m666 ${ROMFS_DIR}/dev/mtd4       c  90  8
	mknod -m666 ${ROMFS_DIR}/dev/mtd4ro     c  90  9
	mknod -m666 ${ROMFS_DIR}/dev/mtd5       c  90 10
	mknod -m666 ${ROMFS_DIR}/dev/mtd5ro     c  90 11
	if [ "$_ROOTFS_IN_FLASH_" = "y" ];then
		mknod -m666 ${ROMFS_DIR}/dev/mtd6       c  90 12
		mknod -m666 ${ROMFS_DIR}/dev/mtd6ro     c  90 13
	fi
	mknod -m666 ${ROMFS_DIR}/dev/ppp        c 108  0
	mknod -m666 ${ROMFS_DIR}/dev/flash0     c 200  0
#	mknod -m666 ${ROMFS_DIR}/dev/rdm0       c 254  0  #for reg tool
	mknod -m666 ${ROMFS_DIR}/dev/rdm0       c 240  0  #for reg tool
#	mknod -m666 ${ROMFS_DIR}/dev/WISP_LED   c 166 14
	if [ "$_MODEL_" = "Customer" -o "$_MODEL_" = "EW7478AC" ]; then
		mknod -m666 ${ROMFS_DIR}/dev/reset_but	c	167 "$_HW_BUTTON_RESET_"
		mknod -m666 ${ROMFS_DIR}/dev/PowerLED	c 166 "$_HW_LED_POWER_"
		mknod -m666 ${ROMFS_DIR}/dev/GBAND_LED	c 166 "$_HW_LED_WIRELESS_GBAND_"
		mknod -m666 ${ROMFS_DIR}/dev/ABAND_LED	c 166 "$_HW_LED_WIRELESS_ABAND_"
		mknod -m666 ${ROMFS_DIR}/dev/WlanLED	c 166 "$_HW_LED_WIRELES_"
		mknod -m666 ${ROMFS_DIR}/dev/WPS_LED	c 166 "$_HW_LED_WPS_"
		mknod -m666 ${ROMFS_DIR}/dev/LAN_LED	c 166 "$_HW_LED_LAN_"
	elif [ "$_MODEL_" = "BR6278AC" ]; then
		mknod -m666 ${ROMFS_DIR}/dev/OpenVPN_LED  c 166 "$_HW_LED_OPEN_VPN_"
		mknod -m666 ${ROMFS_DIR}/dev/NewFW_LED    c 166 "$_HW_LED_NEW_FW_"
		mknod -m666 ${ROMFS_DIR}/dev/ABAND_LED    c 166 "$_HW_LED_WIRELESS_ABAND_"
		mknod -m666 ${ROMFS_DIR}/dev/GBAND_LED    c 166 "$_HW_LED_WIRELESS_GBAND_"
		mknod -m666 ${ROMFS_DIR}/dev/Internet_LED c 166 "$_HW_LED_INTERNET_"
		mknod -m666 ${ROMFS_DIR}/dev/reset_but    c 167 "$_HW_BUTTON_RESET_"
	else
		mknod -m666 ${ROMFS_DIR}/dev/WLAN_LED	c 166 "$_HW_LED_WIRELESS_"
		mknod -m666 ${ROMFS_DIR}/dev/PowerLED   c 166 "$_HW_LED_POWER_"
		mknod -m666 ${ROMFS_DIR}/dev/reset_but  c 167 "$_HW_BUTTON_RESET_"
		mknod -m666 ${ROMFS_DIR}/dev/wps_but    c 167 "$_HW_BUTTON_WPS_"
		mknod -m666 ${ROMFS_DIR}/dev/switch_but c 167 "$_HW_BUTTON_SWITCH_"
		mknod -m666 ${ROMFS_DIR}/dev/ABAND_LED	c 166 "$_HW_LED_WIRELESS_ABAND_"
		mknod -m666 ${ROMFS_DIR}/dev/GBAND_LED	c 166 "$_HW_LED_WIRELESS_GBAND_"
		if [ "$_POWER_DET_CONTROL_" = 'y' ];then
			mknod -m666 ${ROMFS_DIR}/dev/POWER_DET   c 167 "$_HW_LED_USB_0_"
			mknod -m666 ${ROMFS_DIR}/dev/POWER_EN   c 166 "$_HW_LED_USB_1_"
		else
			mknod -m666 ${ROMFS_DIR}/dev/USB_LED0   c 166 "$_HW_LED_USB_0_"
			mknod -m666 ${ROMFS_DIR}/dev/USB_LED1   c 166 "$_HW_LED_USB_1_"
		fi
		mknod -m666 ${ROMFS_DIR}/dev/WPS_LED    c 166 "$_HW_LED_WPS_"
		mknod -m666 ${ROMFS_DIR}/dev/USB_RESET  c 166 "$_HW_USB_RESET_"
	fi
	mknod -m666 ${ROMFS_DIR}/dev/spiS0 c 217 0
	mknod -m666 ${ROMFS_DIR}/dev/watchdog c 10 130
fi
