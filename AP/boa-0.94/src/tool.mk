#!/bin/sh
. ../../../define/FUNCTION_SCRIPT
. ../../../define/PATH

make -f Makefile-tools clean
make -f Makefile-tools
if [ $? != 0 ]; then
	echo "make tools fail!!"
	exit 1
fi

echo "copy config and webpages to image..."
if [ "$_CONFIG_FILE_NAME_" = "" ]; then
	CONF_FILE="config-GW-${_MODE_}-${_MODEL_}-AC.txt"
else
	CONF_FILE="config-GW-${_MODE_}-${_CONFIG_FILE_NAME_}-AC.txt"
fi

if [ ! -f ../default_value/${CONF_FILE} ]; then
	echo -e "\033[35;1m"
	echo ""
	echo "******************** Can't Find File!! [Path=$CONF_FILE] ********************"
	echo ""
	echo -e "\033[0m"
	exit 1
fi

rm -rf ${IMAGE_DIR}/config.bin
./cvcfg -r ../default_value/${CONF_FILE} ${IMAGE_DIR}/config.bin
if [ $? != 0 ]; then
  echo "cvcfg ${CONF_FILE} fail!!"
  exit 1
fi

rm -rf config-pc.bin setting.bin
./cvcfg -r ../default_value/${CONF_FILE} -op config-pc.bin
  if [ $? != 0 ]; then
    echo "cvcfg ${CONF_FILE} fail!!"
    exit 1
  fi
cp config-pc.bin setting.bin

echo "end copy ${CONF_FILE}"
																
