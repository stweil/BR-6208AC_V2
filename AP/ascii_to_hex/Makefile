include ../../make.def
include ../../define/FUNCTION_COMPILER
include ../../define/PATH

EXEC = ascii_to_hex

#CFLAGS += -I$(LINUX_DIR)/drivers/char -I$(LINUX_DIR)/include
#CFLAGS += $(MODEL) $(FUNCTION)
all: $(EXEC)

$(EXEC): $(EXEC).c
	$(CC) -o $@ $@.c
	$(STRIP) $@
romfs:
	$(ROMFSINST) /bin/$(EXEC)

clean:
	-rm -f $(EXEC) *.o

