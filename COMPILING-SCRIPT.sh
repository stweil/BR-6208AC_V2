#!/bin/sh
. tools/script_tools.sh
STR_DATE1=`date`
T_PLATFORM=${1}
shift;T_CUSTOMER=${1}
shift;T_FIRMWAREVERSION=${1}
shift;T_SDKVERSION=${1}
shift;T_SDKVERSION2=${1}
rm -rf define
rm -rf make.def
if [ "$T_PLATFORM" = "BR6278AC" ]; then
cp -rf define_BR6208ACv2 define
cat make_BR6208ACv2.def > make.def
elif [ "$T_PLATFORM" = "EW7478AC" ]; then
if [ "$T_SDKVERSION2" = "Romaing" ]; then
cp -rf define_EW7478ACR define
cat make_EW7478ACR.def > make.def
else
cp -rf define_EW7478AC define
cat make_EW7478AC.def > make.def
fi
fi
. define/PATH
. define/FUNCTION_SCRIPT
. define/DO_LIST
function SEARCH_AND_REPLACE {
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
else
makemenuconfigflag=1
echo "$target_new" >> $filename
fi
fi
}
DRAW_BOX 80 "Prepare tools" 33
cd ${TOOLS_DIR}
toolchain_name="buildroot-gcc342"
if [ ! -f ${toolchain_name}_ready ] || [ ! -d ${toolchain_name} ]; then
rm -rf ${toolchain_name}_ready ${toolchain_name}
tar jxf ${toolchain_name}.tar.bz2
> ${toolchain_name}_ready
cd squashfs4.2/squashfs-tools/
make
cd ../..
cd lzma-4.65/CPP/7zip/Compress/LZMA_Alone/
make -f makefile.gcc
cd ../../../../..
fi 
cd clean-space
rm -rf clean-space
gcc -o clean-space clean-space.c
cd ..
cd mkimage
rm -rf mkimage *.o
make
cd ..
DRAW_BOX 80 "These applications will be compiled" 33
cd ${APPLICATIONS_DIR}
column_width=26
column_number=3
current_column=0
max_display_length=`expr $column_width - 5`
for i in `ls`; do
if [ ! -f $i/DoIt.sh ]; then
continue
fi
i=`expr substr $i 1 $max_display_length`
echo -n $i
DRAW `expr $column_width - length $i - 4` "."
in_do_list=0
for j in $DO_LIST; do
if [ "$j" = "$i" ]; then
in_do_list=1
break
fi
done
if [ "$in_do_list" = "1" ]; then
echo -ne "[\033[32;1m*\033[0m]  "
else
echo -n "[ ]  "
fi
if [ "$current_column" = "`expr $column_number - 1`" ]; then
current_column=0
echo ""
else
current_column=`expr $current_column + 1`
fi
done
cd ${ROOT_DIR}
DRAW_BOX 80 "Set WEB Directory" 33
rm -f ${GOAHEAD_DIR}/web
rm -f ${BOA_DIR}/web
if [ "$_MODEL_" = "BR6278AC" ]; then
WEB_DIR="web-6278gw-${T_CUSTOMER}"
elif [ "$_EW7478ACR_" = "y" ]; then
WEB_DIR="web-7478ACR-${T_CUSTOMER}"
else
WEB_DIR="web-7478repeater-${T_CUSTOMER}"
fi
if [ -d "${BOA_DIR}/${WEB_DIR}" ]; then
ln -s ${BOA_DIR}/${WEB_DIR} ${BOA_DIR}/web
cp -f ${ROOT_DIR}/define/FUNCTION_SCRIPT ${BOA_DIR}/${WEB_DIR}
echo "Set WEB DIR to ${WEB_DIR} successfully..."
else
echo -e "\033[31;1mCan not find ${WEB_DIR}...\033[0m"
exit 0
fi
DRAW_BOX 80 "Set Wireless Driver Directory" 33
cd kernel_module
./link.sh $T_SDKVERSION
cd $ROOT_DIR
sleep 3
DRAW_BOX 80 "Generate Kernel configure file" 33
cd ${LINUX_DIR}
if [ "$_MODEL_" = "BR6278AC" ]; then
echo ".config.br6278ac"
cp ../.config.br6278ac .config
elif [ "$_EW7478ACR_" = "y" ]; then
echo ".config.ew7478acr"
cp ../.config.ew7478acr .config
else
echo ".config.ew7478ac"
cp ../.config.ew7478ac .config
fi
if [ "$_MODEL_" = "EW7478AC" ]; then
if [ "$_GBAND_SKU_ENABLE_" = "y" -o "$_ABAND_SKU_ENABLE_" = "y" ]; then
SEARCH_AND_REPLACE .config "CONFIG_SINGLE_SKU_V2" "CONFIG_SINGLE_SKU_V2=y"
SEARCH_AND_REPLACE .config "CONFIG_RT2860V2_SINGLE_SKU" "CONFIG_RT2860V2_SINGLE_SKU=y"
else
SEARCH_AND_REPLACE .config "CONFIG_SINGLE_SKU_V2" "# CONFIG_SINGLE_SKU_V2 is not set"
SEARCH_AND_REPLACE .config "CONFIG_RT2860V2_SINGLE_SKU" "# CONFIG_RT2860V2_SINGLE_SKU is not set"
fi
SEARCH_AND_REPLACE .config "CONFIG_DFS_SUPPORT" "# CONFIG_DFS_SUPPORT is not set"
elif [ "$_MODEL_" = "BR6278AC" ]; then
SEARCH_AND_REPLACE .config "CONFIG_RT2860V2_SINGLE_SKU" "# CONFIG_RT2860V2_SINGLE_SKU is not set"
SEARCH_AND_REPLACE .config "CONFIG_MT7610_AP_SINGLE_SKU" "# CONFIG_MT7610_AP_SINGLE_SKU is not set"
fi
if [ $? != 0 ]; then
exit 1
fi
./set_kernel_config.sh
if [ "$_RFCHIP_" = "MT7620" ]; then
rm -fr ${LINUX_DIR}/include/asm
rm -fr ${LINUX_DIR}/arch/mips/include/asm/rt2880
rm -fr ${LINUX_DIR}/include/linux/autoconf.h
ln -sf ../arch/mips/include/asm ${LINUX_DIR}/include/asm
ln -sf ./mach-ralink ${LINUX_DIR}/arch/mips/include/asm/rt2880
ln -sf ../../include/generated/autoconf.h ${LINUX_DIR}/include/linux/autoconf.h
fi
DRAW_BOX 80 "Build Linux Kernel and Modules" 33
rm -f ${APPLICATIONS_DIR}/module/alg/*.* 2> /dev/null
rm -f ${APPLICATIONS_DIR}/module/led/*.* 2> /dev/null
cd ${LINUX_DIR}
rm -rf scripts/setlocalversion
cp -rf scripts/setlocalversion_org scripts/setlocalversion
./DoModule.sh make
if [ $? != 0 ]; then
echo "Build Linux Module Error"
exit 1
fi
DRAW_BOX 80 "Build Applications" 33
cd ${APPLICATIONS_DIR}
echo "$T_PROVIDER" > last_compile_temp
if [ ! -f last_compile ] || [ "`diff last_compile last_compile_temp`" ]; then
rm -f last_compile
./DoApp.sh all
if [ $? = 1 ]; then exit 1; fi
mv -f last_compile_temp last_compile
else
./DoApp.sh make
if [ $? = 1 ]; then exit 1; fi
./DoApp.sh install
if [ $? = 1 ]; then exit 1; fi
fi
cd ${LINUX_DIR}
./DoLinux.sh make
if [ $? != 0 ]; then
echo "Build Linux Kernel Error"
exit 1
fi
DRAW_BOX 80 "Build Image" 33
cd ${IMAGE_DIR}
if [ "${4}" = "STAR" ]; then
./STAR_DoImage.sh
else
./DoImage.sh
fi
if [ $? != 0 ];then
exit 1
fi
STR_DATE2=`date`
echo "Start Time -->"${STR_DATE1}
echo "End Time   -->"${STR_DATE2}
