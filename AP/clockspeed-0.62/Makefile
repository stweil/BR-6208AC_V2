#
# EDX Start
# .sub_build.base is prepared at check_env time
#
-include .sub_build.base
#
# EDX End
#

ifneq ($(CC),)
$(shell echo $(CC) -O2 > conf-cc)
$(shell echo $(CC) -s > conf-ld)
endif

TARGET=sntpclock

.PHONY: $(TARGET)

all: $(TARGET)
	$(Q)$(MAKE) $(MAKEFLAGS) -f Makefile.org $(TARGET)

clean:
	rm -f *.a *.o $(TARGET)

#
# EDX Start
#
distclean:
	$(Q)rm -f $(SUB_BUILD_FILE)

pre_romfs: all
	$(Q)$(STRIP) $(TARGET) -o $(ROMFS_DIR)/bin/$(TARGET)

make_env:
	$(Q)ln -sf $(ABS_SUB_BUILD_FILE) $(SUB_BUILD_FILE)
#
# EDX End
#
