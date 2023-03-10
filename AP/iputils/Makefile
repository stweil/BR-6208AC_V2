# Path to parent kernel include files directory
#KERNEL_INCLUDE=/usr/src/linux/include
#LIBC_INCLUDE=/usr/include
include ../../define/PATH

KERNEL_INCLUDE=$(LINUX_DIR)/include
#LIBC_INCLUDE=/usr/mips-linux-uclibc/include
DEFINES= 

#options if you have a bind>=4.9.4 libresolv (or, maybe, glibc)
LDLIBS=-lresolv
ADDLIB=

ifeq ($(LIBC_INCLUDE)/socketbits.h,$(wildcard $(LIBC_INCLUDE)/socketbits.h))
  ifeq ($(LIBC_INCLUDE)/net/if_packet.h,$(wildcard $(LIBC_INCLUDE)/net/if_packet.h))
    GLIBCFIX=-Iinclude-glibc -include include-glibc/glibc-bugs.h
  endif
endif
ifeq ($(LIBC_INCLUDE)/bits/socket.h,$(wildcard $(LIBC_INCLUDE)/bits/socket.h))
  GLIBCFIX=-Iinclude-glibc -include include-glibc/glibc-bugs.h
endif


#options if you compile with libc5, and without a bind>=4.9.4 libresolv
# NOT AVAILABLE. Please, use libresolv.

include ../../make.def
#CC=gcc
#CC=mips-uclibc-gcc
#LIBC_INCLUDE=:clude
# What a pity, all new gccs are buggy and -Werror does not work. Sigh.
#CCOPT=-D_GNU_SOURCE -O2 -Wstrict-prototypes -Wall -g -Werror
CCOPT=-D_GNU_SOURCE -Os -Wstrict-prototypes -Wall
CFLAGS=$(CCOPT) $(GLIBCFIX) -I$(KERNEL_INCLUDE) -I../include $(DEFINES) 

#IPV4_TARGETS=tracepath ping clockdiff rdisc arping tftpd rarpd
#IPV4_TARGETS=tracepath ping clockdiff rdisc arping tftpd
IPV4_TARGETS=rdisc
#IPV6_TARGETS=tracepath6 traceroute6 ping6
IPV6_TARGETS=
TARGETS=$(IPV4_TARGETS) $(IPV6_TARGETS)

all: check-kernel $(TARGETS)
	$(STRIP) rdisc

tftpd: tftpd.o tftpsubs.o
ping: ping.o ping_common.o
ping6: ping6.o ping_common.o
ping.o ping6.o ping_common.o: ping_common.h
tftpd.o tftpsubs.o: tftp.h


check-kernel:
ifeq ($(KERNEL_INCLUDE),)
	@echo "Please, set correct KERNEL_INCLUDE"; false
else
	@set -e; \
	if [ ! -r $(KERNEL_INCLUDE)/linux/autoconf.h ]; then \
		echo "Please, set correct KERNEL_INCLUDE"; false; fi
endif

modules: check-kernel
	$(MAKE) KERNEL_INCLUDE=$(KERNEL_INCLUDE) -C Modules

man:
	$(MAKE) -C doc man

html:
	$(MAKE) -C doc html

clean:
	rm -f *.o $(TARGETS)
	$(MAKE) -C Modules clean
	$(MAKE) -C doc clean

install: html
	$(MAKE) -C doc install

snapshot: clean
	@if [ ! -r RELNOTES.xxyyzz ]; then echo "Where are RELNOTES?"; exit 1; fi
	@cp RELNOTES RELNOTES.bak
	@date "+[%y%m%d]" > RELNOTES
	@cat RELNOTES.xxyyzz >> RELNOTES
	@cat RELNOTES.bak >> RELNOTES
	@date "+static char SNAPSHOT[] = \"%y%m%d\";" > SNAPSHOT.h
	@$(MAKE) -C doc snapshot
	@rm -f RELNOTES.xxyyzz RELNOTES.bak

