EXEC = switch  
CFLAGS += -I$(ROOTDIR)/$(LINUXDIR)
CFLAGS += -I$(ROOTDIR)/$(LINUXDIR)/drivers/net/raeth
#EDX CONF_H  = $(ROOTDIR)/$(LINUXDIR)/include/linux/autoconf.h
CONF_H  = $(LINUXDIR)/include/linux/autoconf.h #EDX

SRC=switch_gsw.c

all: $(EXEC)
switch: $(SRC) $(CONF_H)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(SRC) $(LDLIBS)

romfs:
	$(ROMFSINST) /bin/switch

clean:
	-rm -f $(EXEC) *.elf *.gdb *.o

