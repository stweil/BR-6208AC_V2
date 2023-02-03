include ../../make.def
include ../../define/FUNCTION_COMPILER
include ../../define/PATH

EXEC = mode_sw

#CFLAGS += -I$(LINUX_DIR)/drivers/char -I$(LINUX_DIR)/include
#CFLAGS += $(MODEL) $(FUNCTION)
all: $(EXEC)

$(EXEC): $(EXEC).c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $@.c $(LDLIBS) $(FUNCTION)
	$(STRIP) $@
romfs:
	$(ROMFSINST) /bin/$(EXEC)

clean:
	-rm -f $(EXEC) *.elf *.gdb *.o

