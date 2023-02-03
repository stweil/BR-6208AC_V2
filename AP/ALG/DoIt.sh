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
	target_dir=${ROMFS_DIR}/bin/alg

	conntrack_prefix=${LINUX_DIR}/net/netfilter/nf_conntrack
	nat_prefix=${LINUX_DIR}/net/ipv4/netfilter/nf_nat

	mkdir -p ${target_dir}
	cp ${conntrack_prefix}*.ko ${target_dir}
	if [ $? = 1 ]; then exit 1; fi
	cp ${nat_prefix}*.ko ${target_dir}
	if [ $? = 1 ]; then exit 1; fi
fi
