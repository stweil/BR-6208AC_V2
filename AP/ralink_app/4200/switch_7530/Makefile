-include .sub_build.base
include /home/BR6477AC_Edimax/define/PATH
include $(LINUX_DIR)/.config

LINUXDIR=$(LINUX_DIR)

EXEC = switch_7530  
CFLAGS += -I$(LINUXDIR)
CFLAGS += -I$(LINUXDIR)/include
CFLAGS += -I$(LINUXDIR)/drivers/net/raeth
CONF_H  = $(LINUXDIR)/include/linux/autoconf.h
TARGET = $(EXEC)

ifeq ($(CONFIG_RALINK_MT7620),y)
SRC=switch_7530.c
endif

all: $(EXEC)
switch: $(SRC) $(CONF_H)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(SRC) $(LDLIBS)

romfs:
	$(ROMFSINST) /bin/switch_7530

clean:
	-rm -f $(EXEC) *.elf *.gdb *.o

%.d: %.c
	@set -e; rm -f $@; \
		$(CC) -c -MM $(CFLAGS) $< | sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' > $@

-include $(OBJS:.o=.d)

distclean: clean
	$(Q)rm -f $(SUB_BUILD_FILE)

clean:
	$(Q)rm -f *.o $(OBJS) $(TARGET) *.d

make_env:
	ln -sf $(ABS_SUB_BUILD_FILE) $(SUB_BUILD_FILE)

pre_romfs install: all
	$(Q)$(STRIP) $(TARGET) -o $(PREBUILD_SBIN)/$(TARGET)
