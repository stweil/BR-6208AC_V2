include ../../make.def
include ../../define/FUNCTION_COMPILER

CFLAGS = -Wall -O2 $(FUNCTION)
#LFLAG  = --static
OBJS   = setup.o md5.o
TAR    = setup

all: $(TAR)

$(TAR): $(OBJS)
	$(CC) $(CFLAGS) $(LFLAG) $(OBJS) -o $(TAR)
	$(STRIP) $@

.c.o:
	$(CC) -c $(CFLAGS) $*.c

clean:
	rm -f *.o $(TAR)
