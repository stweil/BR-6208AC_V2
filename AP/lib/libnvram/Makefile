#
# (C) Copyright 2002
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

TOPDIR=../
include $(TOPDIR)Rules.mak

CFLAGS+=$(SSP_ALL_CFLAGS) -DUSE_HOSTCC
CFLAGS+=-I$(TOPDIR)zlib

LIBNVRAM=libnvram.a
LIBNVRAM_SHARED=libnvram.so
LIBNVRAM_SHARED_FULLNAME=libnvram-$(MAJOR_VERSION).$(MINOR_VERSION).$(SUBLEVEL).so

OBJS	:= crc32.o nvram_env.o flash_api.o
HEADERS := nvram_env.h nvram.h flash_api.h

all:	$(LIBNVRAM)

$(OBJS): %.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@
	$(STRIPTOOL) -x -R .note -R .comment $*.o

shared: all
	$(LD) $(LDFLAGS) -soname=$(LIBNVRAM_SHARED).$(MAJOR_VERSION) \
		-o $(LIBNVRAM_SHARED_FULLNAME) --whole-archive $(LIBNVRAM) \
		--no-whole-archive $(TOPDIR)libc/misc/internals/interp.o \
		-L$(TOPDIR)lib -lc $(LDADD_LIBFLOAT) $(LIBGCC);
	$(INSTALL) -d $(TOPDIR)lib
	$(RM) $(TOPDIR)lib/$(LIBNVRAM_SHARED_FULLNAME) $(TOPDIR)lib/$(LIBNVRAM_SHARED).$(MAJOR_VERSION)
	$(INSTALL) -m 644 $(LIBNVRAM_SHARED_FULLNAME) $(TOPDIR)lib
	$(LN) -sf $(LIBNVRAM_SHARED_FULLNAME) $(TOPDIR)lib/$(LIBNVRAM_SHARED)
	$(LN) -sf $(LIBNVRAM_SHARED_FULLNAME) $(TOPDIR)lib/$(LIBNVRAM_SHARED).$(MAJOR_VERSION)

$(LIBNVRAM) ar-target: $(OBJS)
	$(AR) $(ARFLAGS) $(LIBNVRAM) $(OBJS)
	$(INSTALL) -d $(TOPDIR)lib
	$(RM) $(TOPDIR)lib/$(LIBNVRAM)
	$(INSTALL) -m 644 $(LIBNVRAM) $(TOPDIR)lib

$(OBJS): $(HEADERS) Makefile

clean:
	$(RM) *.[oa] $(LIBNVRAM_SHARED)* $(LIBNVRAM_SHARED_FULLNAME)*

