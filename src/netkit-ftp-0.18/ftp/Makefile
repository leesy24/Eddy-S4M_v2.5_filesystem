all: ftp

include ../MCONFIG
include ../MRULES

USE_READLINE=0

ifeq ($(USE_READLINE),1)
CFLAGS += -D__USE_READLINE__
LIBS += -lreadline $(LIBTERMCAP)
endif

CC=/opt/lemonix/cdt/bin/arm-linux-gcc

ftp: cmds.o cmdtab.o domacro.o ftp.o glob.o main.o ruserpass.o
	$(CC) $(LDFLAGS) $^ $(LIBS) -o $@

domacro.o ftp.o glob.o main.o ruserpass.o: ftp_var.h pathnames.h
cmds.o cmdtab.o: ftp_var.h pathnames.h cmds.h
cmds.o glob.o: glob.h

# CKLEE mak
install: ftp
	/opt/lemonix/cdt/bin/arm-linux-strip ftp
	install -m$(BINMODE) ftp $(INSTALLROOT)$(BINDIR)
#	ln -sf ftp $(INSTALLROOT)$(BINDIR)/pftp
#	install -m$(MANMODE) ftp.1 $(INSTALLROOT)$(MANDIR)/man1
#	ln -sf ftp.1 $(INSTALLROOT)$(MANDIR)/man1/pftp.1
#	install -m$(MANMODE) netrc.5 $(INSTALLROOT)$(MANDIR)/man5

clean:
	rm -f *.o ftp

