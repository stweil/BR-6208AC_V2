#
# (C) Copyright 2000-2004
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
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

include $(TOPDIR)/config.mk

# CFLAGS += -DET_DEBUG -DDEBUG

LIB	= libdrivers.a

OBJS	= rt2880_eth.o #i2c_drv.o

ifeq ($(RALINK_MDIO_ACCESS_FUN),ON)
OBJS	+= mii_mgr.o
endif

ifeq ($(CFG_ENV_IS), IN_FLASH)
OBJS	+= spi_drv.o 
endif

ifeq ($(CFG_ENV_IS), IN_SPI)
 ifeq ($(RT6855A_MP), y)
 OBJS	+= spi_rt6855A_flash.o 
 else
 OBJS	+= spi_flash.o 
 endif
 ifeq ($(RT3883_MP), y)
 OBJS	+= spi_drv.o 
 endif
endif

ifeq ($(CFG_ENV_IS), IN_NAND)
OBJS	+= nand_flash.o 
 ifeq ($(RT3883_MP), y)
 OBJS	+= spi_drv.o 
 endif
endif

ifeq ($(RALINK_OHCI), ON)
OBJS	+= ohci-hcd.o
endif

ifeq ($(RALINK_EHCI), ON)
OBJS	+= ehci-hcd.o ehci-ra.o
endif
 
ifeq ($(RALINK_EPHY_TESTER), ON)
OBJS   += phy_val.o 
endif
ifeq ($(MTK_MSDC), ON)
OBJS	+= msdc/mmc_core.o msdc/mmc_test.o msdc/msdc.o msdc/msdc_cust.o msdc/ts_msdc.o
endif
#EDX jerry start
ifeq ($(MAC_TO_REALTEK_MODE), n)
CFLAGS	+= -D_LITTLE_ENDIAN
SWITCH_OBJS= realtek/smi.o realtek/rtk_api.o realtek/rtl8370_asicdrv.o realtek/rtl8370_asicdrv_vlan.o realtek/rtl8370_asicdrv_dot1x.o realtek/rtl8370_asicdrv_led.o
else
SWITCH_OBJS=
endif
#EDX jerry end

all:	$(LIB)

#EDX $(LIB): $(OBJS)
#EDX	$(AR) crv $@ $(OBJS)
#EDX start
$(LIB): $(OBJS) $(SWITCH_OBJS)
	$(AR) crv $@ $(OBJS) $(SWITCH_OBJS)
#EDX end


#########################################################################

#EDX .depend:	Makefile $(OBJS:.o=.c)
#EDX		$(CC) -M $(CFLAGS) $(OBJS:.o=.c) > $@
#EDX start
.depend:	Makefile $(OBJS:.o=.c) $(SWITCH_OBJS:.o=.S)
		$(CC) -M $(CFLAGS) $(OBJS:.o=.c) $(SWITCH_OBJS:.o=.S) > $@
#EDX end

sinclude .depend

#########################################################################
