#
# pppd makefile for Linux
# $Id: Makefile,v 1.1 2004/08/12 11:00:28 ysc Exp $
#
include ../../../define/FUNCTION_COMPILER
# Default installation locations
BINDIR = /usr/sbin
MANDIR = /usr/man

PPPDSRCS = main.c magic.c fsm.c lcp.c ipcp.c upap.c chap.c md5.c ccp.c ecp.c \
	   ipxcp.c auth.c options.c sys-linux.c md4.c chap_ms.c cbcp.c \
	   demand.c utils.c tty.c sha1.c
HEADERS =  callout.h pathnames.h patchlevel.h chap.h md5.h chap_ms.h md4.h \
	   ipxcp.h cbcp.h tdb.h sha1.h
MANPAGES = pppd.8
PPPDOBJS = main.o magic.o fsm.o lcp.o ipcp.o upap.o chap.o md5.o ccp.o ecp.o \
	   auth.o options.o demand.o utils.o sys-linux.o ipxcp.o tty.o sha1.o

all: pppd

#
# include dependencies if present
ifeq (.depend,$(wildcard .depend))
include .depend
endif

include ../../../make.def

# CC = gcc
#CC= mips-uclibc-gcc
#STRIP=mips-uclibc-strip
#
COPTS = -O2 -pipe -Wall -g  
LIBS = ./plugins/rp-pppoe/libplugin.a

ifneq ($(wildcard /usr/lib/libcrypt.*),)
LIBS += -lcrypt
endif

# Uncomment the next 2 lines to include support for Microsoft's
# MS-CHAP authentication protocol.  Also, edit plugins/radius/Makefile.linux.
CHAPMS=y
USE_CRYPT=y
# Uncomment the next line to include support for MPPE.  CHAPMS (above) must
# also be enabled.  Also, edit plugins/radius/Makefile.linux.
MPPE=y
ifneq ($(wildcard /usr/lib/libcrypt.*),)
HAVE_CRYPT_H=y
endif

# Uncomment the next line to include support for PPP packet filtering.
# This requires that the libpcap library and headers be installed
# and that the kernel driver support PPP packet filtering, which it
# doesn't yet.
FILTER=y

# Uncomment the next line to enable multilink PPP (enabled by default)
# Linux distributions: Please leave multilink ENABLED in your builds
# of pppd!
#HAVE_MULTILINK=y

# Uncomment the next line to enable the TDB database (enabled by default.)
# If you enable multilink, then TDB is automatically enabled also.
# Linux distributions: Please leave TDB ENABLED in your builds.
#USE_TDB=y

#HAS_SHADOW=y
#USE_PAM=y
#HAVE_INET6=y

PLUGIN=y

INCLUDE_DIRS= -I../include -I/home/RT3662/tools/buildroot-gcc342/include

COMPILE_FLAGS= -D_linux_=1 -DHAVE_PATHS_H -DHAVE_MMAP -DCOMPACK_SIZE
#-DIPX_CHANGE 
CFLAGS= $(COPTS) $(COMPILE_FLAGS) $(INCLUDE_DIRS) $(FUNCTION)

ifdef CHAPMS
CFLAGS   += -DCHAPMS=1
ifndef USE_CRYPT
LIBS     := -ldes $(LIBS)
else
CFLAGS   += -DUSE_CRYPT=1
ifneq ($(wildcard /usr/include/crypt.h),)
CFLAGS   += -DHAVE_CRYPT_H=1
endif
endif
PPPDOBJS += md4.o chap_ms.o
ifdef MSLANMAN
CFLAGS   += -DMSLANMAN=1
endif
ifdef MPPE
CFLAGS   += -DMPPE=1
endif
endif

ifdef HAS_SHADOW
CFLAGS   += -DHAS_SHADOW
#LIBS     := -lshadow $(LIBS)
endif

# For "Pluggable Authentication Modules", see ftp.redhat.com:/pub/pam/.
ifdef USE_PAM
CFLAGS   += -DUSE_PAM
LIBS     := -lpam -ldl $(LIBS)
endif

# Multi-linnk
ifdef HAVE_MULTILINK
	# Multilink implies the use of TDB
	USE_TDB=y

	CFLAGS += -DHAVE_MULTILINK
	PPPDSRCS += multilink.c
	PPPDOBJS += multilink.o
endif

# TDB
ifdef USE_TDB
	CFLAGS += -DUSE_TDB=1
	PPPDSRCS += tdb.c
	PPPDOBJS += tdb.o
endif

# Lock library binary for Linux is included in 'linux' subdirectory.
ifdef LOCKLIB
LIBS     := -llock $(LIBS)
CFLAGS   += -DLOCKLIB=1
endif

ifdef PLUGIN
CFLAGS	+= -DPLUGIN
LDFLAGS	+= -Wl,-E
LIBS	+= -ldl
endif

ifdef FILTER
LIBS    += ../../libpcap-0.7.2/libpcap.a
CFLAGS  += -DPPP_FILTER -I../../libpcap-0.7.2

#ifdef FILTER
#LIBS    += -lpcap
#CFLAGS  += -DPPP_FILTER -I/usr/include/pcap
endif

ifdef HAVE_INET6
     PPPDSRCS += ipv6cp.c eui64.c
     HEADERS  += ipv6cp.h eui64.h
     PPPDOBJS += ipv6cp.o eui64.o
     CFLAGS   += -DINET6=1
endif


INSTALL= install -o root

install: pppd
	mkdir -p $(BINDIR) $(MANDIR)
	$(INSTALL) -s -c -m 555 pppd $(BINDIR)/pppd
	if chgrp pppusers $(BINDIR)/pppd 2>/dev/null; then \
	  chmod o-rx,u+s $(BINDIR)/pppd; fi
	$(INSTALL) -c -m 444 pppd.8 $(MANDIR)/man8

pppd: $(PPPDOBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o pppd $(PPPDOBJS) $(LIBS)
	$(STRIP) $@

clean:
	rm -f $(PPPDOBJS) pppd *~ #* core

depend:
	$(CPP) -M $(CFLAGS) $(PPPDSRCS) >.depend
