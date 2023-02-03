include ../../../make.def
#CC     = gcc
#CC     	= /usr/local/gcc333/lexra-nnop-v5/bin/mips-uclibc-gcc
#STRIP	= /usr/local/gcc333/lexra-nnop-v5/bin/mips-uclibc-strip
#INC    = -I../nflash/include
#CFLAGS = -Wall -O2 $(INC)
CFLAGS = -Wall -O2 $(MODEL)

ifeq ($(MODEL),-D_7206GA_)
	CFLAGS += --static
endif

#LIBS   = ../nflash/lib/nflashlib.a
OBJS   = agent.o
TAR    = agent

all: $(TAR)

$(TAR): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TAR)
	$(STRIP) $@

.c.o:
	$(CC) -c $(CFLAGS) $*.c

clean:
	rm -f *.o $(TAR)
