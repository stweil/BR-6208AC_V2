#!  /bin/sh

#. ../../target.def
. ../../define/PATH

if [ $# = 1 ]; then
	case "$1" in
		"clean")
			MAKECLEAR=1
			MAKING=0
			;;
		"make")
			MAKECLEAR=0
			MAKING=1
			;;
    esac
fi  

if [ "$MAKECLEAR" = 1 ]; then
	echo "===== polarssl start (make clean)======"
	cd ../polarssl-1.2.12
		rm -rf build
		make clean
	cd -
	echo "===== polarssl end ===================="

	make clean
	if [ $? != 0 ]; then
		exit 1
	fi				
fi

if [ "$MAKING" = 1 ]; then
	echo "===== polarssl start (make install)===="
	cd ../polarssl-1.2.12
		export SHARED=y
		CC=$CROSS_COMPILE"gcc" make
		make install
	cd -
	echo "===== polarssl end ===================="

	make clean
	export CC=$CROSS_COMPILE""gcc
	export AR=$CROSS_COMPILE""ar
	export AS=$CROSS_COMPILE""as
	export LD=$CROSS_COMPILE""ld
	export RANLIB=$CROSS_COMPILE""ranlib
	export NM=$CROSS_COMPILE""-nm
	export ARCH=mips

	CURLPATH=`pwd`
	SSL_DIR=${APPLICATIONS_DIR}/polarssl-1.2.12/build
	CFLAGS="-I$SSL_DIR/include" 
	LDFLAGS="-L$SSL_DIR/libary" 

	CC=$CROSS_COMPILE""gcc ./configure --host=mips-linux --prefix=$CURLPATH/install --disable-debug --enable-optimize --disable-curldebug --enable-http --disable-ftp --disable-file --disable-ldap --disable-ldaps --disable-rtsp --disable-dict --disable-telnet --disable-tftp --disable-pop3 --disable-imap --disable-smtp --disable-gopher --disable-ipv6 --disable-sspi --enable-crypto-auth --disable-cookies --disable-libcurl-option --disable-shared --without-ssl --with-polarssl=$SSL_DIR

	make
	make install
	if [ $? != 0 ]; then
		exit 1
	fi
fi

