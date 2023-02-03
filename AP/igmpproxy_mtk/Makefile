DIRS = src 

include ../../make.def
include ../../define/PATH
#CONFIG_PRODUCT=MT7620
ROOTDIR=$(ROOT_DIR)
LINUXDIR=linux-2.6.36.x_4200

export CONFIG_PRODUCT ROOTDIR LINUXDIR CC CLAG 



all:
	for i in $(DIRS) ; do make -C $$i $@ || exit $?; done

romfs:
	for i in $(DIRS) ; do make -C $$i $@ ; done

clean:
	for i in $(DIRS) ; do make -C $$i clean ; done

