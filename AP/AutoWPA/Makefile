all:	AutoWPA

include ../../make.def
include ../../define/FUNCTION_COMPILER

#CC	= mips-uclibc-gcc
#STRIP	= mips-uclibc-strip

CFLAGS	= $(FUNCTION)
IFLAGS	=
LDFLAGS =

ifeq ($(SLINK),1)
CFLAGS  += -ffunction-sections -fdata-sections
LDFLAGS += --static -Wl,--gc-sections
endif

ifeq ($(_GENERATE_ENCRYPTSEED_USEMD5_),y)
CT_MD5_O = ct_md5.o
CT_MD5_C = ct_md5.c
endif

DEBUG	= -Wall -Os

OBJS: $(CT_MD5_O) AutoWPA.o

AutoWPA: Makefile $(CT_MD5_O) AutoWPA.o
	$(OBJS)
	$(CC) -o $@ $(DEBUG) $(CFLAGS) $(IFLAGS) $(LDFLAGS) $(CT_MD5_O) AutoWPA.o
	$(STRIP) $@

clean:
	rm -f AutoWPA *.o 

UDPserver.o: AutoWPA.c $(CT_MD5_C)
	$(CC) -c -o $@ $(DEBUG) $(CFLAGS) $(IFLAGS) $<
