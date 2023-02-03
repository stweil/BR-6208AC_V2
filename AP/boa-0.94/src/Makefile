# $Id: Makefile,v 1.5 2010/02/26 01:48:38 keith_huang Exp $

.SUFFIXES:
.SUFFIXES: .o .c
.PHONY: clean mrclean distclean all dist depend

# The following gcc warning switches are too noisy to be useful checking
# Boa for lint:
#  -Wtraditional -Wconversion -Wredundant-decls -Wunreachable-code
# The following gcc warning switches should generate no warnings:
GCC_FLAGS = -Wstrict-prototypes -Wpointer-arith -Wcast-align -Wcast-qual \
  -Wshadow -Waggregate-return -Wmissing-prototypes -Wnested-externs \
  -Wall -W -Wno-unused -Winline -Wwrite-strings -Wundef -pedantic

srcdir = .
VPATH = ./../extras
LDFLAGS =  -g 
SYSCONFIG_DIR=./system

include ../../../make.def
include ../../../define/FUNCTION_COMPILER
include ../../../define/FUNCTION_SCRIPT
include ../../../define/PATH
WPA = -DWLAN_WPA
UNIREPEATER = -DUNIVERSAL_REPEATER
ENDIAN = -D_LITTLE_ENDIAN_
#ENDIAN =

# User Management switch
UMSW    = -DUSER_MANAGEMENT_SUPPORT

# # Digest Access switch
DASW    = -DDIGEST_ACCESS_SUPPORT



LIBS = -lcrypt 
SOAPLIB =
#CFLAGS = -g -O2 -pipe -Wall -DDISABLE_DEBUG
#for size issue
#CFLAGS = -Os -pipe -DDISABLE_DEBUG 
CFLAGS = -Os -pipe -DUSE_AUTH -DHOME_GATEWAY 
CPPFLAGS =  -I. -I.
DEPEND = .depend

#GWSUPP	= -DHOME_GATEWAY -DWIFI_SIMPLE_CONFIG -D_BOA_
GWSUPP	= -DWIFI_SIMPLE_CONFIG -D_BOA_
CFLAGS	+= -DUEMF -DOS="LINUX" -DLINUX $(GWSUPP) 
CFLAGS  += $(WPA) $(UNIREPEATER) $(UMSW) $(DASW) $(SSLSW) $(IFMODSW) $(ENDIAN) $(MODEL)  $(FUNCTION) $(FLASH_RESERVED) $(WLDEV) $(PSDEV) $(SDEV)


# 
# Extra Feature Configuration
#
CFLAGS += $(EXTRA_CONFIG_CFLAGS)

ifeq ($(x86),1)
GWSUPP	+= -DNO_ACTION -DX86_SIMULATE -DEMBED
DEBUG	+= -g
CC		= gcc 
else
LDFLAGS	= -s
CFLAGS += -D__TARGET_BOARD__
endif

CPP = gcc -E

SOURCES = auth.c alias.c boa.c buffer.c cgi.c cgi_header.c config.c escape.c \
	get.c hash.c ip.c log.c mmap_cache.c pipe.c queue.c range.c \
	read.c request.c response.c signals.c util.c sublog.c \
	select.c \
	asp_page.c fmget.c version.c utility.c \
	fmgetiNIC.c fmiNIC.c fmoem_get.c fmoem_set.c \
        fmtcpip.c fmmgmt.c fmwlan.c fmfwall.c fmroute.c md5c.c apform.c   

OBJS = $(SOURCES:.c=.o) timestamp.o 


ifeq ($(_LOCAL_SEARCH_), "y")
JANSSON_LIBS +=  -L$(JANSSONDIR)/_install/lib -ljansson -lm 
JANSSON_FLAGS += -I$(JANSSONDIR)/_install/include
endif


all:	boa 

SOURCES += apmib.c mibtbl.c
OBJS += apmib.o mibtbl.o

boa:	$(OBJS) $(APMIB_LIB)
	$(CC) -o $@ $(APMIB_LIB) $^ $(LDFLAGS) $(LIBS) $(SOAPLIB)

firewall: ${FIREWALL_OBJS} $(APMIB_LIB)
	$(CC) -o setfirewall $(DEBUG) $(CFLAGS) $(IFLAGS) ${FIREWALL_OBJS} $(APMIB_LIB) $(LDFLAGS)
	
ifeq ($(_LOCAL_SEARCH_), "y")	
life_local_agent:
	${CC} -c -o life_local_agent.o $(DEBUG) -DWEBS $(CFLAGS) $(IFLAGS) $(JANSSON_FLAGS) life_local_agent.c
endif
	
boa_indexer:	index_dir.o escape.o   
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

mgbin:	mgbin.o
	gcc $(CFLAGS) mgbin.c -o $@ $(LDFLAGS) $(LIBS)

cvimg:	cvimg.o
	gcc $(CFLAGS) cvimg.c -o $@ $(LDFLAGS) $(LIBS)	

mibtbl_x86.o: mibtbl.c
	gcc -c -o $@ $(DEBUG) $(CFLAGS) -DX86_SIMULATE $(IFLAGS) $<

cvcfg.o: cvcfg.c
	gcc -c -o $@ $(DEBUG) $(CFLAGS) -DX86_SIMULATE $(IFLAGS) $<

cvcfg:	cvcfg.o mibtbl_x86.o
	gcc -o $@ $(DEBUG) $(CFLAGS) $(IFLAGS) cvcfg.o mibtbl_x86.o $(LDFLAGS)

clean:
	rm -f *.o *.so boa cvimg mgbin core *~ 
	
distclean:	mrclean

mrclean:	clean
	rm -f config.status config.cache config.h Makefile config.log

# timestamp

timestamp.o:	$(SOURCES)

# depend stuff
depend: $(SOURCES)
	$(CPP) $(CPPFLAGS) -MM $^ select.c poll.c access.c > $(DEPEND)
        
-include $(DEPEND)


# tags
tags:	$(SOURCES)
	ctags -o tags $^ *.h

# object dump
boa.objdump:    boa
	objdump --disassemble-all --source boa > $@

.c.o:
	${CC} -c -o $@ $(DEBUG) -DWEBS $(CFLAGS) $(IFLAGS) $<
