########################################################
#  MakeFile for RTL871X_TOOL.                          #
#                                                      #
########################################################

CONFIG_MP871X_DBG = n

CC      = /opt/lemonix/cdt/bin/arm-linux-gcc
CCFLAGS = -Wall
#BUILDFLAGS = -static
#LIB     = -lpthread
LIB     = -pthread

object  = rtl871x_mp_ioctl.o r871x_tools.o
output  = mp8712

ifeq ($(CONFIG_MP871X_DBG), y)
CCFLAGS += -DCONFIG_MP871X_DBG
object += r871x_tools_dbg.o
endif

GENOBJ = $(CC) $(CCFLAGS) -c


.KEEP_STATE:

all: build

build: $(object)
	$(CC) $(CCFLAGS) $(BUILDFLAGS) $(LIB) $(object) -o $(output)

# =============== object generate =======================

rtl871x_mp_ioctl.o: rtl871x_mp_ioctl.c rtl871x_mp_ioctl.h
	$(GENOBJ) rtl871x_mp_ioctl.c

r871x_tools.o: r871x_tools.c r871x_tools.h rtl871x_mp_ioctl.h
	$(GENOBJ) r871x_tools.c

ifeq ($(CONFIG_MP871X_DBG), y)
r871x_tools_dbg.o: r871x_tools_dbg.c r871x_tools_dbg.h rtl871x_mp_ioctl.h
	$(GENOBJ) r871x_tools_dbg.c
endif

# =============== object generate =======================

clean:
	rm -f *.o *~ $(output)
