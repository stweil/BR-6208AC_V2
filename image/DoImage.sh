#!/bin/sh
. ../define/PATH
. ../define/FUNCTION_SCRIPT
. ../define/SCRIPT_DEFINED
WEB_DIR=$BOA_DIR
rm -rf linux.comp
rm -rf linux.comp.pad
rm -rf linux.comp.pad.appimg
rm -rf linux.comp.pad.appimg.mkimage
kernel_pad=0x11FFAC # 0x120000 - (64 mkimage_head) - (20 cvimg_head) = 0x11FFAC
kernel_address=80000000
kernel_entry=$(readelf -h vmlinux | grep "Entry" | awk '{print $4}')
${CROSS_COMPILE}objcopy -O binary -R .note -R .comment -S vmlinux linux
${TOOLS_DIR}/lzma-4.65/CPP/7zip/Compress/LZMA_Alone/lzma e linux linux.comp
objcopy -I binary -O binary --pad-to=${kernel_pad} linux.comp linux.comp.pad
cat linux.comp.pad appimg > linux.comp.pad.appimg
${TOOLS_DIR}/mkimage/mkimage -A mips -O linux -T kernel -C lzma -a $kernel_address -e $kernel_entry -n "Linux Kernel Image" -d linux.comp.pad.appimg linux.comp.pad.appimg.mkimage
${BOA_DIR}/src/cvimg vmlinux linux.comp.pad.appimg.mkimage linux.bin 0x1100000 0x70000
if [ "$_EW7478ACR_" = "y" ]; then
_MODEL_="EW7478ACR"
fi
chmod 777 linux.bin
mkdir -p ./${_MODEL_}/${_MODE_}/Tester			
cp -f linux.bin ./${_MODEL_}/${_MODE_}/Tester/${_MODEL_}_${_MODE_}_${_VERSION_}_upg.bin
UBOOT_FILE="uboot-${_RFCHIP_}-SPI-${_MEMSIZE_}M-${_MEMBUS_}BIT.bin"
if [ "$_MODEL_" = "EW7478AC" ];then
UBOOT_FILE="uboot-MT7620-SPI-64M-16BIT-newDDR-EW7478AC.bin"
elif [ "$_MODEL_" = "EW7478ACR" ];then
UBOOT_FILE="uboot-MT7620-SPI-64M-16BIT-newDDR-EW7478AC.bin"
elif [ "$_MODEL_" = "BR6278AC" ]; then
UBOOT_FILE="uboot-MT7620-SPI-64M-16BIT-Edimax-BR6278AC.bin"
fi
echo $UBOOT_FILE
if [ ! -f ${UBOOT_FILE} ]; then
echo "boot code not found!!"
exit 1
else
objcopy -I binary -O binary --pad-to=0x50000 ${UBOOT_FILE} uboot-pad.bin
objcopy -I binary -O binary --pad-to=0x20000 config.bin config-pad.bin
linux_size=`stat -c %s linux.bin`
if [ "$_ROMSIZE_" = "16" ];then # 16M flash
objcopy -I binary -O binary --pad-to=0xF90000 linux.bin linux-pad.bin
elif [ "$_ROMSIZE_" = "8" ];then # 8M flash
objcopy -I binary -O binary --pad-to=0x790000 linux.bin linux-pad.bin
elif [ "$_ROMSIZE_" = "4" ];then # 4M flash
objcopy -I binary -O binary --pad-to=0x390000 linux.bin linux-pad.bin
fi
cat uboot-pad.bin config-pad.bin linux-pad.bin > Factory_temp.bin
rm -rf uboot-pad.bin linux-pad.bin config-pad.bin
if [ $? != 0 ]; then
exit 1
fi
chmod 777 Factory_temp.bin
mkdir -p ./${_MODEL_}/${_MODE_}/Factory	
mv -f Factory_temp.bin ./${_MODEL_}/${_MODE_}/Factory/${_MODEL_}_${_MODE_}_${_VERSION_}_mp.bin
fi
ls -l ./${_MODEL_}/${_MODE_}/Tester/${_MODEL_}_${_MODE_}_${_VERSION_}_upg.bin ./${_MODEL_}/${_MODE_}/Factory/${_MODEL_}_${_MODE_}_${_VERSION_}_mp.bin
