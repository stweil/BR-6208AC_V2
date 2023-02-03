NVDIR = $(ROOTDIR)/lib/libnvram
CFLAGS += -I$(NVDIR) 
LDFLAGS += -lnvram
EXEC = ated
SRCS = ated.c

#EXTRA_CFLAGS += -DDBG

all: $(EXEC)

#EDX $(EXEC): $(SRCS) $(ROOTDIR)/$(LINUXDIR)/include/linux/autoconf.h
$(EXEC): $(SRCS) $(LINUXDIR)/include/linux/autoconf.h #EDX
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) -o $@ $@.c $(LDLIBS)

romfs:
	$(ROMFSINST) /bin/$(EXEC)

clean:
	-rm -f $(EXEC) *.o
