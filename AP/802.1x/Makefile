#PLATFORM = 5VT
#PLATFORM = PC
#PLATFORM = STAR
PLATFORM = RT2880

ifeq ($(PLATFORM),RT2880)
CROSS_COMPILE = mipsel-linux-
endif

ifeq ($(PLATFORM),5VT)
CROSS_COMPILE = /opt/crosstool/uClibc_v5te_le_gcc_4_1_1/bin/arm-linux-
endif

ifeq ($(PLATFORM),STAR)
CROSS_COMPILE = /opt/star/tools/arm-linux/bin/arm-linux-
endif

ifeq ($(PLATFORM),PC)
CROSS_COMPILE =
endif

ifeq ($(RFCODE), 0)
RFCODE = 0
else
RFCODE = 1
endif

CC = $(CROSS_COMPILE)gcc

ifndef CFLAGS
CFLAGS = -O2 -Wall -g -D_RFCODE_=$(RFCODE)
endif

# If you want to support multiple radius server for multiple bssid, add following line
CFLAGS +=  -DMULTIPLE_RADIUS

# If you want to debug daemon, add following line
CFLAGS +=  -DDBG 

OBJS =	rtdot1x.o eloop.o eapol_sm.o radius.o md5.o  \
	config.o ieee802_1x.o  \
	sta_info.o   radius_client.o

all: rtdot1xd

rtdot1xd: $(OBJS)
	$(CC) -o rtdot1xd $(OBJS)
	cp rtdot1xd rt2860apd

clean:
	rm -f core *~ *.o rtdot1xd rt2860apd *.d

#
# add by YYHuang@Ralink
romfs:
	$(ROMFSINST) rt2860apd /bin/rt2860apd
	ln -sf rt2860apd $(ROMFSDIR)/bin/rtinicapd

-include $(OBJS:%.o=%.d)
