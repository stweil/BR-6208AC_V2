#!/bin/bash
#Include Function Defined List.
. ../define/FUNCTION_SCRIPT


KERNEL_CONFIG=".config"
TEMP_CONFIG=".temp_config"
NEW_CONFIG=".new_config"
cp -f $KERNEL_CONFIG $NEW_CONFIG

replaceConfigure()
{
srcStr=`echo $1 | sed -e s/\\\///g`
sed -e s/"$srcStr"/"$2"/g $NEW_CONFIG > $TEMP_CONFIG
mv -f $TEMP_CONFIG $NEW_CONFIG
}
enableConfigure()
{
	set_string="`cat $NEW_CONFIG | grep \"^$1=\"`"
	not_set_string="`cat $NEW_CONFIG | grep \"^# $1 is not set\"`"

	if [ "$2" = "" ]; then
		answer="y"
	else
		answer="$2"
	fi

	if [ "$set_string" = "" ]; then
		if [ "$not_set_string" = "" ]; then
			echo "$1=$answer" >> $NEW_CONFIG
		else
			replaceConfigure "# $1 is not set" "$1=$answer"
		fi
	else
		replaceConfigure "$set_string" "$1=$answer"
	fi 
}

disableConfigure()
{
	set_string="`cat $NEW_CONFIG | grep \"^$1=\"`"
	not_set_string="`cat $NEW_CONFIG | grep \"^# $1 is not set\"`"

	if [ "$set_string" != "" ]; then
		replaceConfigure "$set_string" "# $1 is not set"
	elif [ "$not_set_string" == "" ]; then
		echo "# $1 is not set" >> $NEW_CONFIG
	fi 
}

SEARCH_AND_REPLACE()
{
        if [ -f "$1" ]; then
                filename=$1
                target_old=$2
                target_new=$3
                target_line=`cat $filename | grep -n "$2" | cut -f 1 -d :`
                if [ "$target_line" ]; then
                        total_line=`cat $filename | wc -l`
                        cat $filename | head -n `expr $target_line - 1` > ${filename}_temp
                        echo "$target_new" >> ${filename}_temp
                        cat $filename | tail -n `expr $total_line - $target_line` >> ${filename}_temp
                        cat ${filename}_temp > $filename
                        rm -rf ${filename}_temp
                fi
        fi
}

#********************************************************************************************************************


disableConfigure "CONFIG_NAT_LINUX"
disableConfigure "CONFIG_NAT_RCONE"
disableConfigure "CONFIG_NAT_FCONE"
if [ "$_ROUTE_NAT_TYPE_" = "RESTRICT_CONE" ]; then
	enableConfigure "CONFIG_NAT_RCONE"

elif [ "$_ROUTE_NAT_TYPE_" = "FULL_CONE" ]; then
	enableConfigure "CONFIG_NAT_FCONE"

elif [ "$_ROUTE_NAT_TYPE_" = "SEMANTIC_CONE" ]; then
	enableConfigure "CONFIG_NAT_LINUX"

else
	enableConfigure "CONFIG_NAT_LINUX"

fi

disableConfigure "CONFIG_WAN_AT_P0"
disableConfigure "CONFIG_WAN_AT_P4"
if [ "$_SWTYPE_" = "WLLLL" ];then
	enableConfigure "CONFIG_WAN_AT_P0"

elif [ "$_SWTYPE_" = "LLLLW" ];then
	enableConfigure "CONFIG_WAN_AT_P4"

else
	enableConfigure "CONFIG_WAN_AT_P4"

fi

if [ "$_RFCHIP_" = "MT7620" ];then
	if [ "$_ROMTYPE_" = '1' ];then
		disableConfigure "CONFIG_MTD_NAND_RALINK"
		enableConfigure "CONFIG_MTD_SPI_RALINK"
		disableConfigure "CONFIG_MTD_ANY_RALINK"

	elif [ "$_ROMTYPE_" = '2' ];then
		disableConfigure "CONFIG_MTD_NAND_RALINK"
		disableConfigure "CONFIG_MTD_SPI_RALINK"
		enableConfigure "CONFIG_MTD_ANY_RALINK"

	else
		echo "Not support yet!"
		exit -1

	fi

	disableConfigure "CONFIG_RTPCI_AP_CARRIER"
	if [ "$_CARRIER_" != "" ]; then
		enableConfigure "CONFIG_RTPCI_AP_CARRIER"
	else
		disableConfigure "CONFIG_RTPCI_AP_CARRIER"
	fi
	disableConfigure "CONFIG_RT2860V2_AP_DFS"
	disableConfigure "CONFIG_RTPCI_AP_DFS"
	if [ "$_DFS_" = "y" ]; then
		enableConfigure "CONFIG_RT2860V2_AP_DFS"
		enableConfigure "CONFIG_RTPCI_AP_DFS"
	fi
	disableConfigure "CONFIG_EXT2_FS"
	disableConfigure "CONFIG_EXT3_FS"
	disableConfigure "CONFIG_EXT4_FS"
	disableConfigure "CONFIG_MSDOS_PARTITION"
fi
disableConfigure "CONFIG_IPV6"

if [ "$_MCAST_RATE_SUPPORT_" ]; then
	enableConfigure "CONFIG_RT2860V2_MCAST_RATE_SPECIFIC"
	enableConfigure "CONFIG_RT3572_MCAST_RATE_SPECIFIC"
fi

if [ "$_OPENVPN_" ]; then
	enableConfigure "CONFIG_TUN"
fi

#use initramfs or sqshfs
disableConfigure "CONFIG_RT2880_ROOTFS_IN_RAM"
disableConfigure "CONFIG_RT2880_ROOTFS_IN_FLASH"
disableConfigure "CONFIG_ROOTFS_IN_FLASH_NO_PADDING"	
disableConfigure "CONFIG_MTD_KERNEL_PART_SIZ"

disableConfigure "CONFIG_BLK_DEV_INITRD"
disableConfigure "CONFIG_INITRAMFS_SOURCE"
disableConfigure "CONFIG_INITRAMFS_ROOT_UID"
disableConfigure "CONFIG_INITRAMFS_ROOT_GID"
disableConfigure "CONFIG_RD_GZIP"
disableConfigure "CONFIG_RD_BZIP2"
disableConfigure "CONFIG_RD_LZMA"
disableConfigure "CONFIG_RD_XZ"
disableConfigure "CONFIG_RD_LZO"
disableConfigure "CONFIG_INITRAMFS_COMPRESSION_NONE"
disableConfigure "CONFIG_INITRAMFS_COMPRESSION_LZMA"

disableConfigure "CONFIG_MISC_FILESYSTEMS"
disableConfigure "CONFIG_ADFS_FS"
disableConfigure "CONFIG_AFFS_FS"
disableConfigure "CONFIG_HFS_FS"
disableConfigure "CONFIG_HFSPLUS_FS"
disableConfigure "CONFIG_BEFS_FS"
disableConfigure "CONFIG_BFS_FS"
disableConfigure "CONFIG_EFS_FS"
disableConfigure "CONFIG_JFFS2_FS"
disableConfigure "CONFIG_LOGFS"
disableConfigure "CONFIG_CRAMFS"
disableConfigure "CONFIG_SQUASHFS"
disableConfigure "CONFIG_SQUASHFS_XATTR"
disableConfigure "CONFIG_SQUASHFS_LZO"
disableConfigure "CONFIG_SQUASHFS_XZ"
disableConfigure "CONFIG_SQUASHFS_LZMA"
disableConfigure "CONFIG_SQUASHFS_EMBEDDED"
disableConfigure "CONFIG_SQUASHFS_FRAGMENT_CACHE_SIZE"
disableConfigure "CONFIG_VXFS_FS"
disableConfigure "CONFIG_MINIX_FS"
disableConfigure "CONFIG_OMFS_FS"
disableConfigure "CONFIG_HPFS_FS"
disableConfigure "CONFIG_QNX4FS_FS"
disableConfigure "CONFIG_ROMFS_FS"
disableConfigure "CONFIG_SYSV_FS"
disableConfigure "CONFIG_UFS_FS"

disableConfigure "CONFIG_CMDLINE_OVERRIDE"

disableConfigure "CONFIG_ZLIB_INFLATE"
disableConfigure "CONFIG_LZO_DECOMPRESS"
disableConfigure "CONFIG_XZ_DEC"
disableConfigure "CONFIG_XZ_DEC_X86"
disableConfigure "CONFIG_XZ_DEC_POWERPC"
disableConfigure "CONFIG_XZ_DEC_IA64"
disableConfigure "CONFIG_XZ_DEC_ARM"
disableConfigure "CONFIG_XZ_DEC_ARMTHUMB"
disableConfigure "CONFIG_XZ_DEC_SPARC"
disableConfigure "CONFIG_XZ_DEC_BCJ"
disableConfigure "CONFIG_XZ_DEC_TEST"
enableConfigure "CONFIG_INITRAMFS_COMPRESSION_LZMA"
enableConfigure "CONFIG_DECOMPRESS_LZMA"


if [ "$_ROOTFS_IN_FLASH_" ];then
	enableConfigure "CONFIG_RT2880_ROOTFS_IN_FLASH"
	enableConfigure "CONFIG_MTD_KERNEL_PART_SIZ" "0x120000"	

	enableConfigure "CONFIG_MISC_FILESYSTEMS"
	enableConfigure "CONFIG_SQUASHFS"
	enableConfigure "CONFIG_SQUASHFS_XATTR"
	enableConfigure "CONFIG_SQUASHFS_LZMA"
	enableConfigure "CONFIG_SQUASHFS_EMBEDDED"
	enableConfigure "CONFIG_SQUASHFS_FRAGMENT_CACHE_SIZE" "3"
else
	enableConfigure "CONFIG_RT2880_ROOTFS_IN_RAM"
	enableConfigure "CONFIG_BLK_DEV_INITRD"
	enableConfigure "CONFIG_INITRAMFS_SOURCE" "\"..\/romfs\/\""
	enableConfigure "CONFIG_INITRAMFS_ROOT_UID" "0"
	enableConfigure "CONFIG_INITRAMFS_ROOT_GID" "0"
	enableConfigure "CONFIG_RD_LZMA"
fi

#********************************************************************************************************************

CMP_CONFIG=`diff $KERNEL_CONFIG $NEW_CONFIG`
if [ "$CMP_CONFIG" != "" ]; then
	cp -f $NEW_CONFIG $KERNEL_CONFIG
	rm -f include/asm/asm-mips
fi 
echo "Set kernel config file successfully!"    
