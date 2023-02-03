#!/bin/sh
set -e
. ../../define/PATH
. ../../define/FUNCTION_SCRIPT

#Use polarssl
if [ -f Makefile -a "$1" = "clean" ]; then
	make distclean
fi

if [ "$1" = "clean" ]; then 
	echo "===== polarssl start (make clean)======"
	cd ../polarssl-1.2.12
		rm -rf build
		make clean
	cd -
	echo "===== polarssl end ===================="

	exit; 
fi


echo "===== polarssl start (make install)===="
	cd ../polarssl-1.2.12
		export SHARED=y
		CC=$CROSS_COMPILE"gcc" make
		make install
	cd -
echo "===== polarssl end ===================="

SSL_DIR=${APPLICATIONS_DIR}/polarssl-1.2.12/build
OPENVPN_DIR=${APPLICATIONS_DIR}/openvpn-2.3.2/libs4Compile
POLARSSL_CFLAGS="-I${SSL_DIR}/include" 

export RANLIB=${CROSS_COMPILE}ranlib 
export STRIP=${CROSS_COMPILE}strip  
export POLARSSL_LIBS="-L${SSL_DIR}/lib -lpolarssl"
export CFLAGS="-I${OPENVPN_DIR}/include "$POLARSSL_CFLAGS
export LIBS="-L${OPENVPN_DIR}/lib" 

CC=${CROSS_COMPILE}gcc ./configure --host=mips-linux --enable-password-save --prefix=${APPLICATIONS_DIR}/openvpn-2.3.2/openvpn --with-crypto-library=polarssl 

if [ "$_SUPPORT_OPENVPN_HW_CRYPTO_" = "y" ]; then
	export RTK_OPENVPN_HW_CRYPTO="yes"
	cp -f config.h.in.hwCrypto config.h.in
	cp -f configure.hwCrypto configure
else
	cp -f config.h.in.def config.h.in
	cp -f configure.def configure
fi

if [ "$1" = "configure" ]; then exit; fi
rm -f ./libs4Compile/include/security
nowPath=`pwd`
ln -s $nowPath/libs4Compile/include ./libs4Compile/include/security
make
